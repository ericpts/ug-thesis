#include "project.h"
#include <cassert>
#include <memory>
#include <iostream>

std::unique_ptr<Project> _global_p;

Project &project()
{
    assert(_global_p);
    return *_global_p;
}

void set_project(std::unique_ptr<Project> p)
{
    _global_p.swap(p);
}

Project::Project(std::vector<ClassFile> files) : m_files(files)
{
}

std::optional<ClassFile>
Project::resolve_classfile(const std::string &class_name) const
{
    for (const ClassFile &c : this->m_files) {
        if (c->class_name() == class_name) {
            return c;
        }
    }
    return {};
}

std::optional<Method>
Project::resolve_symbolic_reference(const std::string &class_name,
                                    const std::string &method_name,
                                    const std::string &method_type) const
{
    std::optional<ClassFile> maybe_class_file =
        this->resolve_classfile(class_name);
    if (!maybe_class_file.has_value()) {
        return {};
    }
    const ClassFile &cf = maybe_class_file.value();
    for (const Method &m : Method::all_from_classfile(cf)) {
        if (m.method_name() == method_name && m.method_type() == method_type) {
            std::cerr << "Successfully resolved " << method_name  << " :: " << method_type << "\n";
            return m;
        }
    }
    return {};
}

Method Project::main_method() const
{
    std::optional<Method> maybe_main_method;
    for (const ClassFile &file : this->m_files) {
        for (const Method &m : Method::all_from_classfile(file)) {
            std::cerr << m.format() << ", ";
        }
        for (int i = 0; i < file->method_count; ++i) {
            if (!file->cp_index_is_string(file->methods[i].name_index, "main")) {
                std::cerr << "Skipping method " << file->class_name() << "::"
                          << file->cp_index_as_string(file->methods[i].name_index)
                          << "()"
                          << " because it is not main()\n";
                continue;
            }
            // There should be a single main method.
            assert(!maybe_main_method.has_value());
            maybe_main_method = Method::from_owner(file, i, file->methods[i]);
        }
    }
    assert(maybe_main_method.has_value());
    std::cerr << "Found the main file and method.\n";

    return maybe_main_method.value();
}

std::vector<ClassFile> Project::files() const
{
    return this->m_files;
}

void Project::remove_unused_methods()
{
    Method main_method = project().main_method();
    std::vector<Method> reachable_methods = main_method.called_methods();

    auto is_reachable = [&reachable_methods] (const Method& m) -> bool
    {
        if (m.method_name() == "<init>" ||
            m.method_name() == "main") {
            return true;
        }

        for (const Method& o : reachable_methods) {
            if (m == o) {
                return true;
            }
        }
        return false;
    };

    for (ClassFile & cf : this->m_files) {
        std::vector<Method> to_remove;

        for (Method m : Method::all_from_classfile(cf)) {
            if (!is_reachable(m)) {
                to_remove.push_back(m);
            }
        }

        if (to_remove.empty()) {
            continue;
        }

        std::cerr << "Found " << to_remove.size() << " method(s) to remove from class " << cf->class_name() << "\n";
        for (Method m : to_remove) {
            std::cerr << "Removing " << m.format() << "...\n";
            cf = m.refresh(cf).with_this_method_removed();
        }
    }
}
