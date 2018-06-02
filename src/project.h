#pragma once
#include <memory>
#include <optional>
#include <vector>

#include "classfile.h"
#include "method.h"

// A Project is a collection of class files, together with a main entry point.
struct Project {
  private:
    std::vector<ClassFile> m_files;

  public:
    Project(std::vector<ClassFile> files);

    // Find the appropiate class file given the name.
    std::optional<ClassFile>
    resolve_classfile(const std::string &class_name) const;

    std::optional<Method>
    resolve_symbolic_reference(const std::string &class_name,
                               const std::string &method_name,
                               const std::string &method_type) const;

    // Find the main entry point of the entire project.
    Method main_method() const;

    // This is for now not purely functional, due to the global resolution of method references
    // via the `project()` function.
    //
    // As ClassFiles are actively modified for each removed method, all references needs need to be re-resolved
    // after every deletion, because they are reindexed.
    // Due to the way external references work (class A references a method of class B), based on name and type,
    // it is only necessary to update the methods from within the modified ClassFile.
    void remove_unused_methods();

    std::vector<ClassFile> files() const;
};

// For now, there is a single global project for the entire application.
Project &project();
void set_project(std::unique_ptr<Project> p);
