#include "classwriter.h"

ClassWriter::ClassWriter(const ClassFile &cf) : m_cf(cf), m_data()
{
    // Speed up the process by preallocating a decent amount of memory.
    this->m_data.reserve(sizeof(cf));
}

std::vector<uint8_t> ClassWriter::serialize()
{
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

void ClassWriter::put_cp_info(std::pair<cp_info, int> info)
{
}

void ClassWriter::put_attribute_info(attribute_info info)
{
}

void ClassWriter::put_field_info(field_info info)
{
}

void ClassWriter::put_method_info(method_info info)
{
}

void ClassWriter::expect_cpool_entry(int idx, cp_info::Tag tag) const
{
}
