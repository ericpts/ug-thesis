#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <optional>

#include <arpa/inet.h>

using u1 = uint8_t;
using u2 = uint16_t;
using u4 = uint32_t;

// Constant pool info and its subclasses.
struct cp_info;
struct CONSTANT_Methodref_info;


struct method_info;
struct field_info;
struct interface_info;

// Attribute informations, and all of its subclasses.
struct attribute_info;
struct Code_attribute;

// Each item in the constant_pool table must begin with a 1-byte tag indicating
// the kind of cp_info entry.
// The contents of the info array vary with the value of tag.
// The valid tags and their values are included in the tag enum.
// Each tag byte must be followed by two or more bytes giving information about
// the specific constant.
// The format of the additional information varies with the tag value.
//
struct cp_info {
    enum class Tag : u1 {
        CONSTANT_Class = 7,
        CONSTANT_Fieldref = 9,
        CONSTANT_Methodref = 10,
        CONSTANT_InterfaceMethodref = 11,
        CONSTANT_String = 8,
        CONSTANT_Integer = 3,
        CONSTANT_Float = 4,
        CONSTANT_Long = 5,
        CONSTANT_Double = 6,
        CONSTANT_NameAndType = 12,
        CONSTANT_Utf8_info = 1,
        CONSTANT_MethodHandle = 15,
        CONSTANT_MethodType = 16,
        CONSTANT_InvokeDynamic = 18,
    };

    Tag tag;
    std::vector<u1> data;

    bool operator==(const cp_info &other) const;

    /// Returns the number of constant pool slots this constant takes up.
    /// By default, most types of constants only take up 1 slot, but some of them have 2.
    int slots() const;

    // Returns *this* as string.
    std::string as_string() const;

    // This template will be explicitly specialized for the possible types.
    template<typename T>
    T as() const;
};

struct CONSTANT_Methodref_info {
    cp_info::Tag tag;
    u2 class_index;
    u2 name_and_type_index;
};

struct CONSTANT_Class_info {
    cp_info::Tag tag;
    u2 name_index;
};

struct CONSTANT_NameAndType_info {
    cp_info::Tag tag;
    u2 name_index;
    u2 descriptor_index;
};

// Each value in the interfaces array must be a valid index into the
// constant_pool table.
// The constant_pool entry at each value of interfaces[i], where 0 ≤ i <
// interfaces_count,
// must be tagged with cp_info::Tag::cref representing an interface that is a
// direct
// superinterface of this class or interface type, in the left-to-right order
// given
// in the source for the type.
struct interface_info {
    u2 idx;

    bool operator==(const interface_info &other) const;
};

struct field_info {
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    std::vector<attribute_info> attributes;

    bool operator==(const field_info &other) const;
};

struct method_info {
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    std::vector<attribute_info> attributes;

    bool operator==(const method_info &other) const;
};

/// Attributes are used in the ClassFile, field_info, method_info, and
/// Code_attribute structures of the class file format.
/// All attributes have the following general format:
struct attribute_info {
    u2 attribute_name_index;
    u4 attribute_length;
    std::vector<u1> info;

    bool operator==(const attribute_info &other) const;

    // This template will be explicitly specialized for the possible types.
    template<typename T>
    T as() const;
};

struct Code_attribute {
    u2 attribute_name_index;
    u4 attribute_length;

    u2 max_stack;
    u2 max_locals;

    u4 code_length;
    std::vector<u1> code;

    u2 exception_table_length;
    struct exception {
        u2 start_pc;
        u2 end_pc;
        u2 handler_pc;
        u2 catch_type;
    };
    std::vector<exception> exception_table; // of length exception_table_length.
    u2 attributes_count;
    std::vector<attribute_info> attributes; // of length attributes_count.
};
