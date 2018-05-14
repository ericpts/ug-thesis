#include "types.h"

bool cp_info::operator==(const cp_info& other) const
{
    return
        this->tag == other.tag &&
        this->data == other.data;
}


bool interface_info::operator==(const interface_info& other) const
{
    return
        this->idx == other.idx;
}

bool field_info::operator==(const field_info& other) const
{
    return
        this->access_flags     == other.access_flags &&
        this->name_index       == other.name_index &&
        this->descriptor_index == other.descriptor_index &&
        this->attributes_count == other.attributes_count &&
        this->attributes       == other.attributes;
}

bool method_info::operator==(const method_info& other) const
{
    return
        this->access_flags == other.access_flags &&
        this->name_index == other.name_index &&
        this->descriptor_index == other.descriptor_index &&
        this->attributes_count == other.attributes_count &&
        this->attributes == other.attributes;
}

bool attribute_info::operator==(const attribute_info& other) const
{
    return
        this->attribute_name_index == other.attribute_name_index &&
        this->attribute_length == other.attribute_length &&
        this->info == other.info;
}

