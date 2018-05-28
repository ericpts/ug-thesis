#include "method.h"
#include "assert.h"
#include "bytesparser.h"
#include "code.h"

#include <iostream>

Method::Method(const ClassFile &file, int index)
    : m_class_file(&file), m_method_index(index)
{
}

const ClassFile &Method::reference_class_file() const
{
    return *(this->m_class_file);
}

const std::string Method::owner_class_name() const
{
    const CONSTANT_Methodref_info mref =
        this->reference_class_file()
            .constant_pool[this->m_method_index]
            .as<CONSTANT_Methodref_info>();
    const CONSTANT_Class_info cinfo = this->reference_class_file()
                                          .constant_pool[mref.class_index]
                                          .as<CONSTANT_Class_info>();
    const std::string class_name = this->reference_class_file()
                                       .constant_pool[cinfo.name_index]
                                       .as_string();
    return class_name;
}

Code_attribute Method::code_attribute() const
{
    auto maybe_ret =
        this->reference_class_file().code_attribute_for_method_index(
            this->m_method_index);
    assert(maybe_ret.has_value());
    return maybe_ret.value();
}

std::string Method::method_name() const
{
    const CONSTANT_Methodref_info mref =
        this->reference_class_file()
            .constant_pool[this->m_method_index]
            .as<CONSTANT_Methodref_info>();
    const CONSTANT_NameAndType_info ntinfo =
        this->reference_class_file()
            .constant_pool[mref.name_and_type_index]
            .as<CONSTANT_NameAndType_info>();
    const std::string method_name = this->reference_class_file()
                                        .constant_pool[ntinfo.name_index]
                                        .as_string();

    return method_name;
}

std::string Method::format() const
{
    return this->owner_class_name() + "::" + this->method_name() + "()";
}

std::vector<Method> Method::called_methods() const
{
    std::vector<Method> ret;
    Code_attribute attr = this->code_attribute();

    BytesParser bp{attr.code};
    while (!bp.is_end()) {
        Instr i = static_cast<Instr>(bp.next_u1());
        switch (i) {
        case Instr::invokedynamic: {
            std::cerr << "Found function call instruction invokedynamic.\n";
            break;
        }
        case Instr::invokeinterface: {
            std::cerr << "Found function call instruction invokeinterface.\n";
            break;
        }
        case Instr::invokespecial: {
            std::cerr << "Found function call instruction invokespecial.\n";
            break;
        }
        case Instr::invokestatic: {
            std::cerr << "Found function call instruction invokestatic.\n";
            const u2 index = bp.next_u2();
            ret.push_back(Method(this->reference_class_file(), index));
            break;
        }
        case Instr::invokevirtual: {
            std::cerr << "Found function call instruction invokevirtual.\n";
            const u2 index = bp.next_u2();
            ret.push_back(Method(this->reference_class_file(), index));
            break;
        }
        default: {
            break;
        }
        }
    }
    return ret;
}
