#include "classwriter.h"
#include "util.h"

ClassWriter::ClassWriter(const ClassFileImpl &cf) : m_cf(cf), m_data()
{
    // Speed up the process by preallocating a decent amount of memory.
    this->m_data.reserve(sizeof(cf));
}

std::vector<uint8_t> ClassWriter::serialize()
{
    const ClassFileImpl &cf = this->m_cf;
    assert(cf.magic == 0xCAFEBABE);
    this->put_u4(cf.magic);

    this->put_u2(cf.minor_version);
    this->put_u2(cf.major_version);

    this->put_u2(cf.constant_pool_count);
    assert(cf.constant_pool_count == cf.constant_pool.size());
    // The constant pool is indexed from  to ncpool - 1.
    for (auto it = cf.constant_pool.begin() + 1; it != cf.constant_pool.end();
         it += it->slots()) {
        this->put_cp_info(*it);
    }
    debug << "Done putting constants.\n";

    this->put_u2(cf.access_flags);
    this->put_u2(cf.this_class);
    this->put_u2(cf.super_class);

    this->put_u2(cf.interface_count);
    assert(cf.interface_count == cf.interfaces.size());

    for (const interface_info &i : cf.interfaces) {
        expect_cpool_entry(i.idx, cp_info::Tag::CONSTANT_Class);
        this->put_u2(i.idx);
    }
    debug << "Done putting interfaces.\n";

    this->put_u2(cf.field_count);
    assert(cf.field_count == cf.fields.size());
    for (const field_info &f : cf.fields) {
        this->put_field_info(f);
    }
    debug << "Done putting fields.\n";

    this->put_u2(cf.method_count);
    assert(cf.method_count == cf.methods.size());
    for (const method_info &m : cf.methods) {
        this->put_method_info(m);
    }
    debug << "Done putting methods.\n";

    this->put_u2(cf.attribute_count);
    assert(cf.attribute_count == cf.attributes.size());
    for (const attribute_info &attr : cf.attributes) {
        this->put_attribute_info(attr);
    }
    debug << "Done putting attributs.\n";

    return this->m_data;
}

void ClassWriter::put_u1(u1 x)
{
    this->m_data.push_back(x);
}

void ClassWriter::put_u2(u2 x)
{
    u1 p1 = x >> 8u;
    u1 p2 = x - (p1 << 8u);

    this->put_u1(p1);
    this->put_u1(p2);
}

void ClassWriter::put_u4(u4 x)
{
    u2 p1 = x >> 16u;
    u2 p2 = x - (p1 << 16u);

    this->put_u2(p1);
    this->put_u2(p2);
}

void ClassWriter::put_n(const std::vector<u1> &v)
{
    for (u1 x : v) {
        this->put_u1(x);
    }
}

void ClassWriter::put_cp_info(cp_info info)
{
    cp_info::Tag &tag = info.tag;
    std::vector<u1> &data = info.data;

    this->put_u1(static_cast<u1>(tag));

    switch (tag) {
    case cp_info::Tag::CONSTANT_Utf8_info: {
        u2 u = data.size();
        this->put_u2(u);
        this->put_n(data);
        break;
    }
    case cp_info::Tag::CONSTANT_Integer: {
        static_assert(sizeof(int32_t) == 4,
                      "Signed 32 bit integer must be 4 bytes.");
        this->put_n(data);
        break;
    }
    case cp_info::Tag::CONSTANT_Float: {
        static_assert(sizeof(float) == 4,
                      "32 bit floating pointer number must be 4 bytes.");
        this->put_n(data);
        break;
    }
    case cp_info::Tag::CONSTANT_Long: {
        static_assert(sizeof(int64_t) == 8,
                      "Signed 64 bit integer must be 8 bytes.");
        this->put_n(data);
        break;
    }
    case cp_info::Tag::CONSTANT_Double: {
        static_assert(sizeof(double) == 8,
                      "64 bit floating point number must be 8 bytes.");
        this->put_n(data);
        break;
    }
    case cp_info::Tag::CONSTANT_Class: {
        this->put_n(data);
        break;
    }
    case cp_info::Tag::CONSTANT_String: {
        this->put_n(data);
        break;
    }
    case cp_info::Tag::CONSTANT_Fieldref: {
        this->put_n(data);
        break;
    }
    case cp_info::Tag::CONSTANT_Methodref: {
        this->put_n(data);
        break;
    }
    case cp_info::Tag::CONSTANT_InterfaceMethodref: {
        this->put_n(data);
        break;
    }
    case cp_info::Tag::CONSTANT_NameAndType: {
        this->put_n(data);
        break;
    }
    case cp_info::Tag::CONSTANT_MethodHandle: {
        this->put_n(data);
        break;
    }
    case cp_info::Tag::CONSTANT_MethodType: {
        this->put_n(data);
        break;
    }
    case cp_info::Tag::CONSTANT_InvokeDynamic: {
        this->put_n(data);
        break;
    }
    default: {
        assert(false);
    }
    }
}

void ClassWriter::put_attribute_info(attribute_info info)
{
    this->put_u2(info.attribute_name_index);
    this->put_u4(info.attribute_length);

    assert(info.attribute_length == info.info.size());
    this->put_n(info.info);
}

void ClassWriter::put_field_info(field_info f)
{
    this->put_u2(f.access_flags);

    this->put_u2(f.name_index);
    this->expect_cpool_entry(f.name_index, cp_info::Tag::CONSTANT_Utf8_info);

    this->put_u2(f.descriptor_index);
    this->expect_cpool_entry(f.descriptor_index,
                             cp_info::Tag::CONSTANT_Utf8_info);

    this->put_u2(f.attributes_count);
    assert(f.attributes_count == f.attributes.size());
    for (const attribute_info &attr : f.attributes) {
        this->put_attribute_info(attr);
    }
}

void ClassWriter::put_method_info(method_info info)
{
    this->put_u2(info.access_flags);

    this->put_u2(info.name_index);
    this->expect_cpool_entry(info.name_index, cp_info::Tag::CONSTANT_Utf8_info);

    this->put_u2(info.descriptor_index);
    this->expect_cpool_entry(info.descriptor_index,
                             cp_info::Tag::CONSTANT_Utf8_info);

    this->put_u2(info.attributes_count);
    assert(info.attributes_count == info.attributes.size());
    for (const attribute_info &attr : info.attributes) {
        this->put_attribute_info(attr);
    }
}

void ClassWriter::expect_cpool_entry(int idx, cp_info::Tag tag) const
{
    assert(1 <= idx && idx <= this->m_cf.constant_pool_count - 1);
    assert(this->m_cf.constant_pool[idx].tag == tag);
}
