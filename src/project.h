#pragma once
#include <vector>
#include <optional>
#include <memory>

#include "classfile.h"
#include "method.h"

// A Project is a collection of class files, together with a main entry point.
struct Project
{
private:
    std::vector<ClassFile> m_files;

public:
    Project(std::vector<ClassFile> files);

    // Find the appropiate class file given the name.
    std::optional<ClassFile> resolve_classfile(const std::string& class_name) const;

    std::optional<Method> resolve_symbolic_reference(
            const std::string& class_name,
            const std::string& method_name,
            const std::string& method_type
            ) const;
};


// For now, there is a single global project for the entire application.
Project& project();
void set_project(std::unique_ptr<Project> p);
