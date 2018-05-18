#include "bytesparser.h"

#include <cassert>

BytesParser::BytesParser(std::vector<u1> data):
    m_data(data),
    m_it(m_data.begin())
{

}

u1 BytesParser::next_u1()
{
    assert(m_it != m_data.end());
    return *(m_it++);
}

// Returns the next unsigned short, in network order.
u2 BytesParser::next_u2()
{
    uint8_t b1 = next_u1();
    uint8_t b2 = next_u1();
    return (b1 << 8u) | b2;
}

// Returns the next unsigned int, in network order.
u4 BytesParser::next_u4()
{
    uint16_t s1 = next_u2();
    uint16_t s2 = next_u2();
    return (s1 << 16u) | s2;
}

std::vector<u1> BytesParser::next_n(int n)
{
    std::vector<u1> ret(n);
    for (u1 &u : ret) {
        u = next_u1();
    }
    return ret;
}

bool BytesParser::is_end() const
{
    return m_it == m_data.end();
}

attribute_info BytesParser::parse_attribute_info()
{
    u2 attribute_name_index = this->next_u2();
    u4 attribute_length = this->next_u4();
    std::vector<u1> info = this->next_n(attribute_length);

    return attribute_info{attribute_name_index, attribute_length, info};
}

