#pragma once
#include <vector>
#include <optional>

#include "classfile.h"

// A Project is a collection of class files, together with a main entry point.
struct Project
{
private:
    std::vector<ClassFile> m_files;

public:
    Project(std::vector<ClassFile> files);

    // Find the appropiate class file given the name.
    std::optional<ClassFile> resolve_classfile(const std::string& class_name) const;
};
