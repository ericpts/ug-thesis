#pragma once

#include "types.h"
#include <memory>
#include <optional>

// ClassFiles are going to be shared across methods.

struct ClassFileImpl;
using ClassFile = std::shared_ptr<const ClassFileImpl>;

struct ClassFileImpl {
  public:
    // field_infos
    u4 magic; // Should be 0xCAFEBABE.

    u2 minor_version;
    u2 major_version;

    u2 constant_pool_count;
    std::vector<cp_info>
        constant_pool; // This is indexed from 1 to constant_pool_count - 1.

    u2 access_flags;

    u2 this_class;
    u2 super_class;

    u2 interface_count;
    std::vector<interface_info> interfaces;

    u2 field_count;
    std::vector<field_info> fields;

    u2 method_count;
    std::vector<method_info> methods;

    u2 attribute_count;
    std::vector<attribute_info> attributes;

  public:
    // Returns whether `index` is actually an index into the constant pool
    // array.
    bool is_cp_index(int index) const;

    // Returns whether `index` is actually an index into the methods array.
    bool is_method_index(int index) const;

    // Returns whether the index `index` is a string, and corresponds to the
    // string `s`.
    // Note that this asserts that the index is actually contained within the
    // constant pool.
    bool cp_index_is_string(int index, const std::string &s) const;

    // Asserts that the constant at index `index` is an utf8 string, and returns
    // it.
    std::string cp_index_as_string(int index) const;

    // Returns the name of the class corresponding to this classfile.
    std::string class_name() const;

    // Returns the name of the super class.
    std::string super_class_name() const;

    // Returns whether this is actually an interface, and not a class.
    bool is_interface() const;

    // Returns whether this .class file represents an actual class, as opposed to an interface.
    bool is_class() const;

    // Returns a vector containing the names of all the interfaces directly implemented by *this*.
    // Note that this does not include transitive dependencies.
    std::vector<std::string> interface_names() const;

  public:
    /// Deserialize the binary data into a class file.
    static ClassFileImpl deserialize(const std::vector<uint8_t> &data);

    /// Serialize the class file into binary data.
    std::vector<uint8_t> serialize() const;
};
