#include "method.h"
#include "assert.h"
#include "bytesparser.h"
#include "code.h"
#include "project.h"

#include <iostream>

Method::Method(ClassFile file, int index)
    : m_class_file(file), m_method_index(index)
{
    assert(this->class_file()->is_method_index(index));
}

Method Method::from_owner(const ClassFile &file, int index, method_info info)
{
    assert(file->is_method_index(index));
    assert(file->methods[index] == info);
    return Method(file, index);
}

std::optional<Method> Method::from_symbolic_reference(const ClassFile &file,
                                                      int cp_index,
                                                      cp_info info)
{
    assert(file->constant_pool[cp_index] == info);

    const CONSTANT_Methodref_info mref =
        file->constant_pool[cp_index].as<CONSTANT_Methodref_info>();
    const CONSTANT_Class_info cinfo =
        file->constant_pool[mref.class_index].as<CONSTANT_Class_info>();
    const std::string class_name =
        file->constant_pool[cinfo.name_index].as_string();

    const CONSTANT_NameAndType_info ntinfo =
        file->constant_pool[mref.name_and_type_index]
            .as<CONSTANT_NameAndType_info>();

    const std::string method_name =
        file->constant_pool[ntinfo.name_index].as_string();
    const std::string method_type =
        file->constant_pool[ntinfo.descriptor_index].as_string();

    std::cerr << "Trying to resolve method from symbolic reference: "
              << "(" << method_name << ") and type " << method_type << ""
              << ".\n";

    std::optional<Method> maybe_method = project().resolve_symbolic_reference(
        class_name, method_name, method_type);

    if (!maybe_method.has_value()) {
        std::cerr << "Could note resolve: "
                  << "(" << method_name << ") and type " << method_type << "."
                  << " It is probably part of a library!\n";
    }
    return maybe_method;
}

std::vector<Method> Method::all_from_classfile(const ClassFile &file)
{
    std::vector<Method> ret;
    for (int i = 0; i < file->method_count; ++i) {
        ret.push_back({file, i});
    }
    return ret;
}

std::optional<Method> Method::main_method(const ClassFile &file)
{
    for (const Method &m : Method::all_from_classfile(file)) {
        if (m.method_name() == "main") {
            return m;
        }
    }
    return {};
}

const ClassFile &Method::class_file() const
{
    return (this->m_class_file);
}

Code_attribute Method::code_attribute() const
{
    const method_info &m = this->class_file()->methods[this->m_method_index];
    for (const attribute_info &attr : m.attributes) {
        if (!this->class_file()->cp_index_is_string(attr.attribute_name_index,
                                                    "Code")) {
            continue;
        }
        Code_attribute code = attr.as<Code_attribute>();
        return code;
    }
    assert(false); // We should find the code.
}

std::string Method::method_name() const
{
    return this->class_file()->cp_index_as_string(
        this->class_file()->methods[this->m_method_index].name_index);
}

std::string Method::method_type() const
{
    return this->class_file()->cp_index_as_string(
        this->class_file()->methods[this->m_method_index].descriptor_index);
}

std::string Method::format() const
{
    return this->class_file()->class_name() + "/" + this->method_name() +
           " :: " + this->method_type();
}

std::vector<Method> Method::called_methods() const
{
    std::vector<Method> ret;
    Code_attribute attr = this->code_attribute();

    auto add_optional = [&ret](std::optional<Method> m) -> void {
        if (m.has_value()) {
            ret.push_back(m.value());
        }
    };

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
            add_optional(Method::from_symbolic_reference(
                this->class_file(), index,
                this->class_file()->constant_pool[index]));
            break;
        }
        case Instr::invokevirtual: {
            std::cerr << "Found function call instruction invokevirtual.\n";
            const u2 index = bp.next_u2();
            add_optional(Method::from_symbolic_reference(
                this->class_file(), index,
                this->class_file()->constant_pool[index]));
            break;
        }
        default: {
            break;
        }
        }
    }
    return ret;
}

ClassFile Method::with_this_method_removed() const
{
    auto ret = std::make_shared<ClassFileImpl>(*this->class_file());
    ret->methods.erase(ret->methods.begin() + this->m_method_index);
    ret->method_count--;
    return ret;
}

bool Method::operator==(const Method &o) const
{
    return this->class_file() == o.class_file() &&
           this->m_method_index == o.m_method_index;
}

Method Method::refresh(const ClassFile &new_file) const
{
    for (Method m : Method::all_from_classfile(new_file)) {
        if (m.format() == this->format()) {
            return m;
        }
    }
    assert(false); // This method was probably deleted, and should not be
                   // refreshed.
}
