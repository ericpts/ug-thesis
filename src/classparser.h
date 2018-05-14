#pragma once

#include <cassert>
#include <vector>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

#include "types.h"
#include "classfile.h"

/// This class handles the parsation (deserialization and serialization) of Java's .class files.
/// Normal usage should be:
/// 1. Reading the binary data (for example, from a file on disk)
/// 2. Instantiating this ClassParser.
/// 3. Parsing the actual file.
struct ClassParser
{
private:
    /// The binary representation of the class being parser.
    const std::vector<uint8_t>& m_data;

    /// Iterator to the current byte being parsed.
    std::vector<uint8_t>::const_iterator m_it;

public:
    /// Initialize the parser, with the binary `data` of the class file.
    ClassParser(const std::vector<uint8_t>& data);

    /// Parse an entire class file.
    /// This is the method that you most likely want to use.
    ClassFile parse_class();

private:
    /// Consumes and returns the next unsigned char, in network order.
    u1 next_u1();

    /// Consumes and returns the next unsigned short, in network order.
    u2 next_u2();

    /// Consumes and returns the next unsigned int, in network order.
    u4 next_u4();

    /// Consumes and returns the next `n` bytes.
    std::vector<u1> next_n(int n);

    /// Parses a constant from the data buffer, and returns the data
    /// and how many slots it takes up in the constant table.
    std::pair<Constant, int> parse_constant();
};
