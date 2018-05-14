#pragma once

#include "types.h"

struct ClassFile
{
public:
    // Fields
    u4 magic; // Should be 0xCAFEBABE.

    u2 minor_version;
    u2 major_version;

    u2 constant_pool_count;
    std::vector<Constant> constant_pool; // This is indexed from 1 to constant_pool_count - 1.

    u2 access_flags;

    u2 this_class;
    u2 super_class;

    u2 interface_count;
    std::vector<Interface> interface_table;

    u2 field_count;
    std::vector<Field> field_table;

    u2 method_count;
    std::vector<Method> method_table;

    u2 attribute_count;
    std::vector<Attribute> attribute_table;

public:
    // Methods.

    /// Deserialize the binary data into a class file.
    static ClassFile deserialize(const std::vector<uint8_t>& data);

    /// Serialize the class file into binary data.
    std::vector<uint8_t> serialize() const;
};

