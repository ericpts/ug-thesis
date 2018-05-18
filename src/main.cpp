#include <CLI11.hpp>
#include <iostream>

#include "classfile.h"
#include "classreader.h"
#include "types.h"
#include "util.h"
#include "bytesparser.h"

enum class Instr {
    invokedynamic = 0xba,
    invokeinterface = 0xb9,
    invokespecial = 0xb7,
    invokestatic = 0xb8,
    invokevirtual = 0xb6,
};

struct Method {
    // The name of the class where this method belongs.
    std::string class_name;
    std::string method_name;
};

std::vector<Method> called_methods(ClassFile file, Code_attribute attr)
{
    std::vector<Method> ret;

    BytesParser bp{attr.code};
    while (!bp.is_end()) {
        Instr i = static_cast<Instr>(bp.next_u1());
        switch(i) {
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
                const u2 index = bp.next_u2();
                const CONSTANT_Methodref_info mref = file.constant_pool[index].as<CONSTANT_Methodref_info>();
                const CONSTANT_Class_info cinfo = file.constant_pool[mref.class_index].as<CONSTANT_Class_info>();
                const std::string class_name = file.constant_pool[cinfo.name_index].as_string();

                const CONSTANT_NameAndType_info ntinfo = file.constant_pool[mref.name_and_type_index].as<CONSTANT_NameAndType_info>();
                const std::string method_name = file.constant_pool[ntinfo.name_index].as_string();

                ret.push_back({class_name, method_name});
                break;
            }
            case Instr::invokevirtual: {
                std::cerr << "Found function call instruction invokevirtual.\n";
                break;
            }
            default: {
                break;
            }
        }
    }

    return ret;
}

int main(int argc, char **argv)
{
    std::string filename = argv[1];
    ClassFile file = ClassFile::deserialize(read_entire_file(filename));

    std::optional<Code_attribute> maybe_code;
    for (int i = 0; i < file.method_count; ++i) {
        if (!file.cp_index_is_string(file.methods[i].name_index, "main")) {
            continue;
        }
        std::cerr << "Found the main method.\n";
        maybe_code = file.code_attribute(i);
    }

    assert (maybe_code.has_value());

    std::vector<Method> methods = called_methods(file, maybe_code.value());
    for (const Method& m : methods) {
        std::cerr << "Calls: " << m.class_name << "/" << m.method_name << "\n";
    }

    return 0;
}
