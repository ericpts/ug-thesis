#include <CLI11.hpp>
#include <iostream>

#include "bytesparser.h"
#include "classfile.h"
#include "classreader.h"
#include "types.h"
#include "util.h"

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

    static Method from_methodref_index(const ClassFile &file, int index);
};

Method Method::from_methodref_index(const ClassFile &file, int index)
{
    const CONSTANT_Methodref_info mref =
        file.constant_pool[index].as<CONSTANT_Methodref_info>();
    const CONSTANT_Class_info cinfo =
        file.constant_pool[mref.class_index].as<CONSTANT_Class_info>();
    const std::string class_name =
        file.constant_pool[cinfo.name_index].as_string();

    const CONSTANT_NameAndType_info ntinfo =
        file.constant_pool[mref.name_and_type_index]
            .as<CONSTANT_NameAndType_info>();
    const std::string method_name =
        file.constant_pool[ntinfo.name_index].as_string();

    Method ret;
    ret.class_name = class_name;
    ret.method_name = method_name;
    return ret;
}

std::vector<Method> called_methods(const ClassFile &file, Code_attribute attr)
{
    std::vector<Method> ret;

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
            ret.push_back(Method::from_methodref_index(file, index));
            break;
        }
        case Instr::invokevirtual: {
            std::cerr << "Found function call instruction invokevirtual.\n";
            const u2 index = bp.next_u2();
            ret.push_back(Method::from_methodref_index(file, index));
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
    std::vector<std::string> filenames(argv + 1, argv + argc);
    std::vector<ClassFile> files;
    for (const std::string &filename : filenames) {
        files.push_back(ClassFile::deserialize(read_entire_file(filename)));
    }

    ClassFile *main_file = nullptr;
    std::optional<Code_attribute> maybe_main;
    for (ClassFile &file : files) {
        for (int i = 0; i < file.method_count; ++i) {
            if (!file.cp_index_is_string(file.methods[i].name_index, "main")) {
                continue;
            }
            std::cerr << "Found the main file and method.\n";

            // There should be a single main method.
            assert(!maybe_main.has_value());
            assert(main_file == nullptr);

            maybe_main = file.code_attribute(i);
            main_file = &file;
        }
    }

    assert(maybe_main.has_value());
    assert(main_file != nullptr);

    std::vector<Method> methods =
        called_methods(*main_file, maybe_main.value());
    for (const Method &m : methods) {
        std::cerr << "Calls: " << m.class_name << "::" << m.method_name
                  << "()\n";
    }

    return 0;
}
