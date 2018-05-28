#include <CLI11.hpp>
#include <iostream>

#include "bytesparser.h"
#include "classfile.h"
#include "classreader.h"
#include "method.h"
#include "types.h"
#include "util.h"
#include "project.h"

int main(int argc, char **argv)
{
    std::vector<std::string> filenames(argv + 1, argv + argc);
    std::vector<ClassFile> files;
    for (const std::string &filename : filenames) {
        files.push_back(ClassFile::deserialize(read_entire_file(filename)));
    }
    Project project(files);

    std::optional<Method> maybe_main_method;
    for (ClassFile &file : files) {
        for (const Method& m : Method::all_from_classfile(file)) {
            std::cerr << m.format() << ", ";
        }
        for (int i = 0; i < file.method_count; ++i) {
            if (!file.cp_index_is_string(file.methods[i].name_index, "main")) {
                std::cerr << "Skipping method " << file.class_name() << "::"
                          << file.cp_index_as_string(file.methods[i].name_index)
                          << "()"
                          << " because it is not main()\n";
                continue;
            }
            std::cerr << "Found the main file and method.\n";
            // There should be a single main method.
            assert(!maybe_main_method.has_value());
            maybe_main_method = Method::from_owner(file, i, file.methods[i]);
        }
    }
    assert(maybe_main_method.has_value());
    std::vector<Method> methods = maybe_main_method.value().called_methods();
    for (const Method &m : methods) {
        std::cerr << "Calls: " << m.format() << "\n";
    }
    return 0;
}
