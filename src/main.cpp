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

    std::vector<std::experimental::filesystem::path> filenames;
    app.add_option("classfiles,--classfiles", filenames,
            "All of the class files from the project")
        ->required()
        ->check(CLI::ExistingFile)
        ->set_type_name("[File]");

    std::optional<std::string> out;
    app.add_option("--out", out, "Where to save the modified class files")
        ->check(CLI::ExistingDirectory)
        ->set_type_name("Dir");

    int in_place;
    app.add_flag("--in-place", in_place, "Whether to save the class files in-place")
        ->excludes("--out");

    CLI11_PARSE(app, argc, argv);

    if (not in_place and not out) {
        std::cout << "Must specify either --in-place or --out!\n";
        return -1;
    }

    set_project(std::make_unique<Project>(filenames));
    project().remove_unused_methods();

    if (out) {
        project().save(out.value());
    } else {
        assert (in_place);
        project().save_in_place(filenames);
    }

    return 0;
}
