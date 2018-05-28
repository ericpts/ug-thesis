#include "classfile.h"
#include "classreader.h"
#include "classwriter.h"

bool ClassFile::cp_index_is_string(int index, const std::string &s) const
{
    return this->cp_index_as_string(index) == s;
}

bool ClassFile::is_cp_index(int index) const
{
    return 1 <= index && index < this->constant_pool_count;
}

bool ClassFile::is_method_index(int index) const
{
    return 0 <= index && index < this->method_count;
}


std::string ClassFile::cp_index_as_string(int index) const
{
    assert(this->is_cp_index(index));
    return this->constant_pool[index].as_string();
}

std::string ClassFile::class_name() const
{
    assert(this->is_cp_index(this->this_class));

    const CONSTANT_Class_info cinfo =
        this->constant_pool[this->this_class].as<CONSTANT_Class_info>();
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
