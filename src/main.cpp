#include <CLI11.hpp>
#include <iostream>

#include "bytesparser.h"
#include "classfile.h"
#include "classreader.h"
#include "method.h"
#include "project.h"
#include "types.h"
#include "util.h"

int main(int argc, char **argv)
{
    std::vector<std::string> filenames(argv + 1, argv + argc);
    std::vector<ClassFile> files;
    for (const std::string &filename : filenames) {
        files.push_back(
                std::make_shared<ClassFileImpl>(
                    ClassFileImpl::deserialize(
                        read_entire_file(filename)))
        );
    }

    set_project(std::make_unique<Project>(files));
    project().remove_unused_methods();

    return 0;
}
