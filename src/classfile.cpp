#include "classfile.h"
#include "classreader.h"
#include "classwriter.h"

bool ClassFileImpl::cp_index_is_string(int index, const std::string &s) const
{
    return this->cp_index_as_string(index) == s;
}

bool ClassFileImpl::is_cp_index(int index) const
{
    return 1 <= index && index < this->constant_pool_count;
}

bool ClassFileImpl::is_method_index(int index) const
{
    return 0 <= index && index < this->method_count;
}

std::string ClassFileImpl::cp_index_as_string(int index) const
{
    assert(this->is_cp_index(index));
    return this->constant_pool[index].as_string();
}

std::string ClassFileImpl::class_name() const
{
    assert(this->is_cp_index(this->this_class));

    const CONSTANT_Class_info cinfo =
        this->constant_pool[this->this_class].as<CONSTANT_Class_info>();
    return this->cp_index_as_string(cinfo.name_index);
}

std::string ClassFileImpl::super_class_name() const
{
    if (this->super_class == 0) {
        return "Object";
    }
    assert(this->is_cp_index(this->super_class));

    const CONSTANT_Class_info cinfo =
        this->constant_pool[this->super_class].as<CONSTANT_Class_info>();
    return this->cp_index_as_string(cinfo.name_index);
}

bool ClassFileImpl::is_interface() const
{
    return this->access_flags & 0x0200;
}

bool ClassFileImpl::is_class() const
{
    return !this->is_interface();
}

std::vector<std::string> ClassFileImpl::interface_names() const
{
    std::vector<std::string> ret;
    for (const interface_info &info : this->interfaces) {
        const CONSTANT_Class_info cinfo =
            this->constant_pool[info.idx].as<CONSTANT_Class_info>();
        ret.push_back(this->cp_index_as_string(cinfo.name_index));
    }
    return ret;
}

ClassFileImpl ClassFileImpl::deserialize(const std::vector<uint8_t> &data)
{
    ClassReader reader(data);
    return reader.deserialize();
}

std::vector<uint8_t> ClassFileImpl::serialize() const
{
    ClassWriter writer(*this);
    return writer.serialize();
}
