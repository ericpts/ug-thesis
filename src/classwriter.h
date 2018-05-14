#pragma once

#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "classfile.h"
#include "types.h"

/// This class handles the serialization of Java's .class files.
/// Normal usage should be:
struct ClassWriter {
  private:
    /// The class file that is used to generate the data.
    const ClassFile &m_cf;

    /// The buffer that is being filled with the serialization data of `m_cf`.
    std::vector<uint8_t> m_data;

  public:
    /// Initialize the parser, with the binary `data` of the class file.
    ClassWriter(const ClassFile &cf);

    /// Parse an entire class file.
    /// This is the method that you most likely want to use.
    std::vector<uint8_t> serialize();

  private:
    /// Puts an unsigned char into the data buffer, in network order.
    void put_u1(u1 x);

    /// Puts an unsigned short into the data buffer, in network order.
    void put_u2(u2 x);

    /// Puts an unsigned int into the data buffer, in network order.
    void put_u4(u4 x);

    /// Puts the given vector into the data buffer.
    void put_n(const std::vector<u1> &x);

    /// Puts a constant from the data buffer, and returns the data
    /// and how many slots it takes up in the constant table.
    void put_cp_info(std::pair<cp_info, int> info);

    /// Puts an attribute_info struct into the data buffer.
    void put_attribute_info(attribute_info info);

    /// Puts a field_info struct into the data buffer.
    void put_field_info(field_info info);

    /// Puts a method_info struct into the data buffer.
    void put_method_info(method_info info);

    /// Asserts that `idx` is an index into the constant pool, tagged with
    /// `tag`.
    void expect_cpool_entry(int idx, cp_info::Tag tag) const;
};
