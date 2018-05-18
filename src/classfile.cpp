#include "classfile.h"
#include "classreader.h"
#include "classwriter.h"

bool ClassFile::cp_index_is_string(int index, const std::string& s) const
{
    return this->cp_index_as_string(index) == s;
}

bool ClassFile::is_cp_index(int index) const
{
    return 1 <= index &&
        index < this->constant_pool_count;
}

std::string ClassFile::cp_index_as_string(int index) const
{
    assert (this->is_cp_index(index));
    return this->constant_pool[index].as_string();
}

std::optional<Code_attribute> ClassFile::code_attribute(int method_index) const
{
    assert (0 <= method_index);
    assert (method_index < this->method_count);
    const method_info& m = this->methods[method_index];
    for (const attribute_info& attr: m.attributes) {
        if (!this->cp_index_is_string(attr.attribute_name_index, "Code")) {
            continue;
        }
        Code_attribute code = attr.as<Code_attribute>();
        return code;
    }
    return {};
}

std::string ClassFile::class_name() const
{
    assert (this->is_cp_index(this->this_class));

    const CONSTANT_Class_info cinfo = this->constant_pool[this->this_class].as<CONSTANT_Class_info>();
    return this->cp_index_as_string(cinfo.name_index);
}

ClassFile ClassFile::deserialize(const std::vector<uint8_t> &data)
{
    ClassReader reader(data);
    return reader.deserialize();
}

std::vector<uint8_t> ClassFile::serialize() const
{
    ClassWriter writer(*this);
    return writer.serialize();
}
