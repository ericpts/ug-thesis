#pragma once
#include <experimental/filesystem>
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

    Project(std::vector<std::experimental::filesystem::path> class_files);

    // Find the appropiate class file given the name.
    std::optional<ClassFile>
    resolve_classfile(const std::string &class_name) const;

    std::optional<Method>
    resolve_symbolic_reference(const std::string &class_name,
                               const std::string &method_name,
                               const std::string &method_type) const;

    // Returns all of the methods which have the same name and type as `m`,
    // and belong to related classes.
    //
    // For instance, a method of a descendant in the inheritance chart, because
    // it might be that that method is called due to polymorphism.
    std::vector<Method> sibling_methods(const Method& m) const;

    // Find the main entry point of the entire project.
    Method main_method() const;

    // This is for now not purely functional, due to the global resolution of
    // method references
    // via the `project()` function.
    //
    // As ClassFiles are actively modified for each removed method, all
    // references needs need to be re-resolved
    // after every deletion, because they are reindexed.
    // Due to the way external references work (class A references a method of
    // class B), based on name and type,
    // it is only necessary to update the methods from within the modified
    // ClassFile.
    void remove_unused_methods();

    // Save all of the class files in the given location.
    void save(std::experimental::filesystem::path path) const;

    // Save all of the files in-place.
    void save_in_place(std::vector<std::experimental::filesystem::path> filenames) const;

    // Returns all of the class files.
    std::vector<ClassFile> classfiles() const;

    // Returns all of the methods which are reachable by a series of calls from the given method.
    std::vector<Method> method_call_graph(const Method& m) const;

private:
    // Private utilities.

    // Returns whether the two classes are in some way related:
    // there is a path in the directed inheritance graph from down to up.
    bool class_is_descendant(
            const ClassFile& down, const ClassFile& up) const;

};

// For now, there is a single global project for the entire application.
Project &project();
void set_project(std::unique_ptr<Project> p);
