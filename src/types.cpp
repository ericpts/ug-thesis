#include "types.h"
#include "bytesparser.h"

#include <cassert>

bool cp_info::operator==(const cp_info &other) const
{
    return this->tag == other.tag && this->data == other.data;
}

int cp_info::slots() const
{
    assert(this->tag);
    if (this->tag == cp_info::Tag::CONSTANT_Long ||
        this->tag == cp_info::Tag::CONSTANT_Double) {
        return 2;
    }
    return 1;
}


bool cp_info::is_string(const std::string& s) const
{
    assert (this->tag);
    return this->tag == Tag::CONSTANT_Utf8_info &&
        std::string(this->data.begin(), this->data.end()) == s;
}

bool interface_info::operator==(const interface_info &other) const
{
    return this->idx == other.idx;
}

bool field_info::operator==(const field_info &other) const
{
    return this->access_flags == other.access_flags &&
           this->name_index == other.name_index &&
           this->descriptor_index == other.descriptor_index &&
           this->attributes_count == other.attributes_count &&
           this->attributes == other.attributes;
}

bool method_info::operator==(const method_info &other) const
{
    return this->access_flags == other.access_flags &&
           this->name_index == other.name_index &&
           this->descriptor_index == other.descriptor_index &&
           this->attributes_count == other.attributes_count &&
           this->attributes == other.attributes;
}

bool attribute_info::operator==(const attribute_info &other) const
{
    return this->attribute_name_index == other.attribute_name_index &&
           this->attribute_length == other.attribute_length &&
           this->info == other.info;
}


template<>
Code_attribute attribute_info::as() const
{
    Code_attribute t;
    t.attribute_name_index = this->attribute_name_index;
    t.attribute_length = this->attribute_length;

    BytesParser bp(this->info);
    t.max_stack = bp.next_u2();
    t.max_locals = bp.next_u2();

    t.code_length = bp.next_u4();
    t.code = bp.next_n(t.code_length);

    t.exception_table_length = bp.next_u2();
    t.exception_table.resize(t.exception_table_length);
    for (Code_attribute::exception& ex: t.exception_table) {
        ex.start_pc = bp.next_u2();
        ex.end_pc = bp.next_u2();
        ex.handler_pc = bp.next_u2();
        ex.catch_type = bp.next_u2();
    }

    t.attributes_count = bp.next_u2();
    t.attributes.resize(t.attributes_count);
    for (attribute_info& attr: t.attributes) {
        attr = bp.parse_attribute_info();
    }

    return t;
}
