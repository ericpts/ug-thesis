#include "method.h"
#include "assert.h"
#include "bytesparser.h"
#include "code.h"

#include <iostream>

Method::Method(const ClassFile &file, int index)
    : m_class_file(&file), m_method_index(index)
{
    assert (this->class_file().is_method_index(index));
}

Method Method::from_owner(const ClassFile &file, int index, method_info info)
{
    assert (file.is_method_index(index));
    assert (file.methods[index] == info);
    return Method(file, index);
}

Method Method::from_symbolic_reference(const ClassFile &file, int cp_index, cp_info info)
{
    assert (file.constant_pool[cp_index] == info);

    const CONSTANT_Methodref_info mref =
        file.constant_pool[cp_index].as<CONSTANT_Methodref_info>();
    const CONSTANT_Class_info cinfo =
        file.constant_pool[mref.class_index].as<CONSTANT_Class_info>();
    const std::string class_name =
        file.constant_pool[cinfo.name_index].as_string();

    const CONSTANT_NameAndType_info ntinfo =
        file.constant_pool[mref.name_and_type_index]
            .as<CONSTANT_NameAndType_info>();
    const std::string method_name =
        file.constant_pool[ntinfo.name_index].as_string();

    assert(false);

    // TODO(ericpts): finish this.
    // Given class_name and method_name, figure out the Method referenced.
}

std::vector<Method> Method::all_from_classfile(const ClassFile& file)
{
	std::vector<Method> ret;
    for (int i = 0; i < file.method_count; ++i) {
        ret.push_back({file, i});
    }
    return ret;
}

std::optional<Method> Method::main_method(const ClassFile& file)
{
    for (const Method& m : Method::all_from_classfile(file)) {
        if (m.method_name() == "main") {
            return m;
        }
    }
    return {};
}

const ClassFile &Method::class_file() const
{
    return *(this->m_class_file);
}

Code_attribute Method::code_attribute() const
{
    const method_info &m = this->class_file().methods[this->m_method_index];
    for (const attribute_info &attr : m.attributes) {
        if (!this->class_file().cp_index_is_string(attr.attribute_name_index, "Code")) {
            continue;
        }
        Code_attribute code = attr.as<Code_attribute>();
        return code;
    }
    assert (false); // We should find the code.
}

std::string Method::method_name() const
{
    return this->class_file().cp_index_as_string(this->class_file().methods[this->m_method_index].name_index);
}

std::string Method::format() const
{
    return this->class_file().class_name() + "::" + this->method_name() + "()";
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
            ret.push_back(Method::from_symbolic_reference(
                        this->class_file(), index, this->class_file().constant_pool[index]));
            break;
        }
        case Instr::invokevirtual: {
            std::cerr << "Found function call instruction invokevirtual.\n";
            const u2 index = bp.next_u2();
            ret.push_back(Method::from_symbolic_reference(
                        this->class_file(), index, this->class_file().constant_pool[index]));
            break;
        }
        default: {
            break;
        }
        }
    }
    return ret;
}
