#include <CLI11.hpp>
#include <iostream>

#include "classfile.h"
#include "classreader.h"
#include "types.h"
#include "util.h"

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

    return 0;
}
