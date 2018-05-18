#include "types.h"

#include <vector>

// This struct handles the byte-by-byte parsing of a byte array.
struct BytesParser {
private:
    std::vector<u1> m_data;
    std::vector<u1>::iterator m_it;

public:
    BytesParser(std::vector<u1> data);

    /// Consumes and returns the next unsigned char, in network order.
    u1 next_u1();

    /// Consumes and returns the next unsigned short, in network order.
    u2 next_u2();

    /// Consumes and returns the next unsigned int, in network order.
    u4 next_u4();

    /// Consumes and returns the next `n` bytes.
    std::vector<u1> next_n(int n);

    /// Returns whether all the data has been parsed.
    bool is_end() const;

    /// Parses an attribute_info struct from the data buffer.
    attribute_info parse_attribute_info();
};
