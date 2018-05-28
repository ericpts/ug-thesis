#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>

#include "classreader.h"

ClassReader::ClassReader(std::vector<uint8_t> data) : m_bparser(data)
{
}

ClassFile ClassReader::deserialize()
{
    ClassFile &cf = this->m_cf;
    cf.magic = m_bparser.next_u4();
    assert(cf.magic == 0xCAFEBABE);

    cf.minor_version = m_bparser.next_u2();
    cf.major_version = m_bparser.next_u2();

    cf.constant_pool_count = m_bparser.next_u2();
    cf.constant_pool.resize(cf.constant_pool_count);
    // The constant pool is indexed from  to ncpool - 1.
    for (auto it = cf.constant_pool.begin() + 1;
         it != cf.constant_pool.end();) {
        *it = parse_cp_info();
        it += it->slots();
    }
    std::cerr << "Done parsing constants.\n";

    cf.access_flags = m_bparser.next_u2();
    cf.this_class = m_bparser.next_u2();
    cf.super_class = m_bparser.next_u2();

    cf.interface_count = m_bparser.next_u2();
    cf.interfaces.resize(cf.interface_count);
    for (interface_info &i : cf.interfaces) {
        i.idx = m_bparser.next_u2();
        expect_cpool_entry(i.idx, cp_info::Tag::CONSTANT_Class);
    }
    std::cerr << "Done parsing interfaces.\n";

    cf.field_count = m_bparser.next_u2();
    cf.fields.resize(cf.field_count);
    for (field_info &f : cf.fields) {
        f = this->parse_field_info();
    }
    std::cerr << "Done parsing fields.\n";

    cf.method_count = m_bparser.next_u2();
    cf.methods.resize(cf.method_count);
    for (method_info &m : cf.methods) {
        m = this->parse_method_info();
    }
    std::cerr << "Done parsing methods.\n";

    cf.attribute_count = m_bparser.next_u2();
    cf.attributes.resize(cf.attribute_count);
    for (attribute_info &attr : cf.attributes) {
        attr = m_bparser.parse_attribute_info();
    }
    std::cerr << "Done parsing attributs.\n";

    assert(m_bparser.is_end()); // We should have no data left.
    return cf;
}

/// Parses a constant from the data buffer, and returns the data
/// and how many slots it takes up in the constant table.
cp_info ClassReader::parse_cp_info()
{
    cp_info c;

    cp_info::Tag &tag = c.tag;
    std::vector<u1> &data = c.data;

    tag = static_cast<cp_info::Tag>(m_bparser.next_u1());
    switch (tag) {
    case cp_info::Tag::CONSTANT_Utf8_info: {
        u2 u = m_bparser.next_u2();
        data = m_bparser.next_n(u);
        break;
    }
    case cp_info::Tag::CONSTANT_Integer: {
        static_assert(sizeof(int32_t) == 4,
                      "Signed 32 bit integer must be 4 bytes.");
        data = m_bparser.next_n(sizeof(int32_t));
        break;
    }
    case cp_info::Tag::CONSTANT_Float: {
        static_assert(sizeof(float) == 4,
                      "32 bit floating pointer number must be 4 bytes.");
        data = m_bparser.next_n(sizeof(float));
        break;
    }
    case cp_info::Tag::CONSTANT_Long: {
        static_assert(sizeof(int64_t) == 8,
                      "Signed 64 bit integer must be 8 bytes.");
        data = m_bparser.next_n(sizeof(int64_t));
        break;
    }
    case cp_info::Tag::CONSTANT_Double: {
        static_assert(sizeof(double) == 8,
                      "64 bit floating point number must be 8 bytes.");
        data = m_bparser.next_n(sizeof(double));
        break;
    }
    case cp_info::Tag::CONSTANT_Class: {
        data = m_bparser.next_n(2);
        break;
    }
    case cp_info::Tag::CONSTANT_String: {
        data = m_bparser.next_n(2);
        break;
    }
    case cp_info::Tag::CONSTANT_Fieldref: {
        data = m_bparser.next_n(4);
        break;
    }
    case cp_info::Tag::CONSTANT_Methodref: {
        data = m_bparser.next_n(4);
        break;
    }
    case cp_info::Tag::CONSTANT_InterfaceMethodref: {
        data = m_bparser.next_n(4);
        break;
    }
    case cp_info::Tag::CONSTANT_NameAndType: {
        data = m_bparser.next_n(4);
        break;
    }
    case cp_info::Tag::CONSTANT_MethodHandle: {
        data = m_bparser.next_n(3);
        break;
    }
    case cp_info::Tag::CONSTANT_MethodType: {
        data = m_bparser.next_n(2);
        break;
    }
    case cp_info::Tag::CONSTANT_InvokeDynamic: {
        data = m_bparser.next_n(4);
        break;
    }
    default: {
        assert(false);
    }
    }
    return c;
}

field_info ClassReader::parse_field_info()
{
    field_info f;
    f.access_flags = m_bparser.next_u2();

    f.name_index = m_bparser.next_u2();
    this->expect_cpool_entry(f.name_index, cp_info::Tag::CONSTANT_Utf8_info);

    f.descriptor_index = m_bparser.next_u2();
    this->expect_cpool_entry(f.descriptor_index,
                             cp_info::Tag::CONSTANT_Utf8_info);

    f.attributes_count = m_bparser.next_u2();
    f.attributes.resize(f.attributes_count);
    for (attribute_info &attr : f.attributes) {
        attr = m_bparser.parse_attribute_info();
    }

    return f;
}

method_info ClassReader::parse_method_info()
{
    u2 access_flags = m_bparser.next_u2();

    u2 name_index = m_bparser.next_u2();
    this->expect_cpool_entry(name_index, cp_info::Tag::CONSTANT_Utf8_info);

    u2 descriptor_index = m_bparser.next_u2();
    this->expect_cpool_entry(descriptor_index,
                             cp_info::Tag::CONSTANT_Utf8_info);

    u2 attributes_count = m_bparser.next_u2();
    std::vector<attribute_info> attributes(attributes_count);
    for (attribute_info &attr : attributes) {
        attr = m_bparser.parse_attribute_info();
    }

    return method_info{access_flags, name_index, descriptor_index,
                       attributes_count, attributes};
}

void ClassReader::expect_cpool_entry(int idx, cp_info::Tag tag) const
{
    assert(1 <= idx && idx <= this->m_cf.constant_pool_count - 1);
    assert(this->m_cf.constant_pool[idx].tag == tag);
};
