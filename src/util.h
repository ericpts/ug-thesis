#pragma once
#include <string>
#include <vector>

#include "types.h"

/// Java project consists of a collection of class files.
struct JavaProject {
    std::vector<std::string> class_files;
};

std::vector<u1> read_entire_file(const std::string &filename);
