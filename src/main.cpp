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
    CLI::App app{"JVM Optimizer"};

    std::vector<std::string> filenames;
    app.add_option("classfiles,--classfiles", filenames,
            "All of the class files from the project")
        ->required()
        ->check(CLI::ExistingFile)
        ->set_type_name("[File]");

    std::string out;
    app.add_option("--out", out, "Where to save the modified class files")
        ->required()
        ->check(CLI::ExistingDirectory)
        ->set_type_name("Dir");

    CLI11_PARSE(app, argc, argv);

    std::vector<ClassFile> files;
    for (const std::string &filename : filenames) {
        files.push_back(std::make_shared<ClassFileImpl>(
            ClassFileImpl::deserialize(read_entire_file(filename))));
    }

    set_project(std::make_unique<Project>(files));
    project().remove_unused_methods();

    project().save(out);

    return 0;
}
