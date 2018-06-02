#pragma once

#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "bytesparser.h"
#include "classfile.h"
#include "types.h"

/// This class handles the parsation (deserialization and serialization) of
/// Java's .class files.
/// Normal usage should be:
/// 1. Reading the binary data (for example, from a file on disk)
/// 2. Instantiating this ClassReader.
/// 3. Parsing the actual file.
struct ClassReader {
  private:
    /// The binary representation of the class being parser.
    BytesParser m_bparser;

    /// The class file that is being populated as the parsing progresses.
    ClassFileImpl m_cf;

  public:
    /// Initialize the reader, with the binary `data` of the class file.
    ClassReader(std::vector<uint8_t> data);

    /// Parse an entire class file.
    /// This is the method that you most likely want to use.
    ClassFileImpl deserialize();

  private:
    /// Parses a constant from the data buffer, and returns the data
    /// and how many slots it takes up in the constant table.
    cp_info parse_cp_info();

    /// Parses a field_info struct from the data buffer.
    field_info parse_field_info();

    /// Parses a method_info struct from the data buffer.
    method_info parse_method_info();

    /// Asserts that `idx` is an index into the constant pool, tagged with
    /// `tag`.
    void expect_cpool_entry(int idx, cp_info::Tag tag) const;
};
