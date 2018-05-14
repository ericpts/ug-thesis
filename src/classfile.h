#pragma once

#include "types.h"

struct ClassFile {
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
    // method_infos.

    /// Deserialize the binary data into a class file.
    static ClassFile deserialize(const std::vector<uint8_t> &data);

    /// Serialize the class file into binary data.
    std::vector<uint8_t> serialize() const;
};
