#include "project.h"
#include <memory>
#include <cassert>

std::unique_ptr<Project> _global_p;

Project& project()
{
    assert (_global_p);
    return *_global_p;
}

void set_project(std::unique_ptr<Project> p)
{
    _global_p.swap(p);
}

Project::Project(std::vector<ClassFile> files):
    m_files(files)
{ }


std::optional<ClassFile> Project::resolve_classfile(const std::string& class_name) const
{
    for (const ClassFile& c : this->m_files) {
        if (c.class_name() == class_name) {
            return c;
        }
    }
    return {};
}


std::optional<Method> Project::resolve_symbolic_reference(
        const std::string& class_name,
        const std::string& method_name,
        const std::string& method_type
        ) const
{
    std::optional<ClassFile> maybe_class_file = this->resolve_classfile(class_name);
    if (!maybe_class_file.has_value()) {
        return {};
    }
    const ClassFile& cf = maybe_class_file.value();
    for (const Method& m : Method::all_from_classfile(cf)) {
        if (m.method_name() == method_name &&
                m.method_type() == method_type) {
            return m;
        }
    }
    return {};
}
