#include "project.h"

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
