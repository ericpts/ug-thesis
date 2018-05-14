#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>

#include "classreader.h"

ClassReader::ClassReader(const std::vector<uint8_t> &data)
    : m_data(data), m_it(m_data.begin())
{
}

ClassFile ClassReader::deserialize()
{
    ClassFile &cf = this->cf;
    cf.magic = this->next_u4();
    assert(cf.magic == 0xCAFEBABE);

    cf.minor_version = this->next_u2();
    cf.major_version = this->next_u2();

    cf.constant_pool_count = this->next_u2();
    cf.constant_pool.resize(cf.constant_pool_count);
    // The constant pool is indexed from  to ncpool - 1.
    for (auto it = cf.constant_pool.begin() + 1;
         it != cf.constant_pool.end();) {
        int nslots;
        std::tie(*it, nslots) = parse_cp_info();
        it += nslots;
    }
    std::cerr << "Done parsing constants.\n";

    cf.access_flags = this->next_u2();
    cf.this_class = this->next_u2();
    cf.super_class = this->next_u2();

    cf.interface_count = this->next_u2();
    cf.interfaces.resize(cf.interface_count);
    for (interface_info &i : cf.interfaces) {
        i.idx = this->next_u2();
        expect_cpool_entry(i.idx, cp_info::Tag::CONSTANT_Class);
    }
    std::cerr << "Done parsing interfaces.\n";

    cf.field_count = this->next_u2();
    cf.fields.resize(cf.field_count);
    for (field_info &f : cf.fields) {
        f = this->parse_field_info();
    }
    std::cerr << "Done parsing fields.\n";

    cf.method_count = this->next_u2();
    cf.methods.resize(cf.method_count);
    for (method_info &m : cf.methods) {
        m = this->parse_method_info();
    }
    std::cerr << "Done parsing methods.\n";

    cf.attribute_count = this->next_u2();
    cf.attributes.resize(cf.attribute_count);
    for (attribute_info &attr : cf.attributes) {
        attr = this->parse_attribute_info();
    }
    std::cerr << "Done parsing attributs.\n";

    assert(m_it == m_data.end()); // We should have no data left.

    return cf;
}

u1 ClassReader::next_u1()
{
    assert(m_it != m_data.end());
    return *(m_it++);
}

// Returns the next unsigned short, in network order.
u2 ClassReader::next_u2()
{
    uint8_t b1 = next_u1();
    uint8_t b2 = next_u1();
    return (b1 << 8u) | b2;
}

// Returns the next unsigned int, in network order.
u4 ClassReader::next_u4()
{
    uint16_t s1 = next_u2();
    uint16_t s2 = next_u2();
    return (s1 << 16u) | s2;
}

std::vector<u1> ClassReader::next_n(int n)
{
    std::vector<u1> ret(n);
    for (u1 &u : ret) {
        u = next_u1();
    }
    return ret;
}

/// Parses a constant from the data buffer, and returns the data
/// and how many slots it takes up in the constant table.
std::pair<cp_info, int> ClassReader::parse_cp_info()
{
    cp_info c;

    cp_info::Tag &tag = c.tag;
    std::vector<u1> &data = c.data;

    // By default, most types of constants only take up one slot.
    int slots = 1;
    tag = static_cast<cp_info::Tag>(this->next_u1());

    switch (tag) {
    case cp_info::Tag::CONSTANT_Utf8_info: {
        u2 u = next_u2();
        data = next_n(u);
        break;
    }
    case cp_info::Tag::CONSTANT_Integer: {
        static_assert(sizeof(int32_t) == 4,
                      "Signed 32 bit integer must be 4 bytes.");
        data = next_n(sizeof(int32_t));
        break;
    }
    case cp_info::Tag::CONSTANT_Float: {
        static_assert(sizeof(float) == 4,
                      "32 bit floating pointer number must be 4 bytes.");
        data = next_n(sizeof(float));
        break;
    }
    case cp_info::Tag::CONSTANT_Long: {
        static_assert(sizeof(int64_t) == 8,
                      "Signed 64 bit integer must be 8 bytes.");
        data = next_n(sizeof(int64_t));
        slots = 2;
        break;
    }
    case cp_info::Tag::CONSTANT_Double: {
        static_assert(sizeof(double) == 8,
                      "64 bit floating point number must be 8 bytes.");
        data = next_n(sizeof(double));
        slots = 2;
        break;
    }
    case cp_info::Tag::CONSTANT_Class: {
        data = next_n(2);
        break;
    }
    case cp_info::Tag::CONSTANT_String: {
        data = next_n(2);
        break;
    }
    case cp_info::Tag::CONSTANT_Fieldref: {
        data = next_n(4);
        break;
    }
    case cp_info::Tag::CONSTANT_Methodref: {
        data = next_n(4);
        break;
    }
    case cp_info::Tag::CONSTANT_InterfaceMethodref: {
        data = next_n(4);
        break;
    }
    case cp_info::Tag::CONSTANT_NameAndType: {
        data = next_n(4);
        break;
    }
    case cp_info::Tag::CONSTANT_MethodHandle: {
        data = next_n(3);
        break;
    }
    case cp_info::Tag::CONSTANT_MethodType: {
        data = next_n(2);
        break;
    }
    case cp_info::Tag::CONSTANT_InvokeDynamic: {
        data = next_n(4);
        break;
    }
    default: {
        assert(false);
    }
    }
    return {c, slots};
}

field_info ClassReader::parse_field_info()
{
    field_info f;
    f.access_flags = this->next_u2();

    f.name_index = this->next_u2();
    this->expect_cpool_entry(f.name_index, cp_info::Tag::CONSTANT_Utf8_info);

    f.descriptor_index = this->next_u2();
    this->expect_cpool_entry(f.descriptor_index,
                             cp_info::Tag::CONSTANT_Utf8_info);

    f.attributes_count = this->next_u2();
    f.attributes.resize(f.attributes_count);
    for (attribute_info &attr : f.attributes) {
        attr = this->parse_attribute_info();
    }

    return f;
}

attribute_info ClassReader::parse_attribute_info()
{
    u2 attribute_name_index = this->next_u2();
    u4 attribute_length = this->next_u4();
    std::vector<u1> info = this->next_n(attribute_length);

    return attribute_info{attribute_name_index, attribute_length, info};
}

method_info ClassReader::parse_method_info()
{
    u2 access_flags = this->next_u2();

    u2 name_index = this->next_u2();
    this->expect_cpool_entry(name_index, cp_info::Tag::CONSTANT_Utf8_info);

    u2 descriptor_index = this->next_u2();
    this->expect_cpool_entry(descriptor_index,
                             cp_info::Tag::CONSTANT_Utf8_info);

    u2 attributes_count = this->next_u2();
    std::vector<attribute_info> attributes(attributes_count);
    for (attribute_info &attr : attributes) {
        attr = this->parse_attribute_info();
    }

    return method_info{access_flags, name_index, descriptor_index,
                       attributes_count, attributes};
}

void ClassReader::expect_cpool_entry(int idx, cp_info::Tag tag) const
{
    assert(1 <= idx && idx <= this->cf.constant_pool_count - 1);
    assert(this->cf.constant_pool[idx].tag == tag);
};
