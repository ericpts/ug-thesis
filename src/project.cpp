#include "project.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>

#include "util.h"

namespace fs = std::experimental::filesystem;

// Writes the binary representation of classfile into the given destination path.
void write_classfile_to_file(const ClassFile& cf, const fs::path& dest);

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

Project::Project(std::vector<fs::path> class_files)
{
    m_files.reserve(class_files.size());
    for (fs::path p : class_files) {
        this->m_files.push_back(std::make_shared<ClassFileImpl>(
            ClassFileImpl::deserialize(read_entire_file(p))));
    }
}

std::optional<ClassFile>
Project::resolve_classfile(const std::string &class_name) const
{
    debug << "Trying to resolve_classfile for " << class_name << "\n";
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
            debug << "Successfully resolved " << method_name
                      << " :: " << method_type << "\n";
            return m;
        }
    }
    return {};
}

Method Project::main_method() const
{
    std::optional<Method> maybe_main_method;
    for (const ClassFile &file : this->m_files) {
        for (int i = 0; i < file->method_count; ++i) {
            if (!file->cp_index_is_string(file->methods[i].name_index,
                                          "main")) {
                debug << "Skipping method " << file->class_name() << "::"
                          << file->cp_index_as_string(
                                 file->methods[i].name_index)
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
    debug << "Found the main file and method.\n";

    return maybe_main_method.value();
}

void Project::remove_unused_methods()
{
    Method main_method = project().main_method();
    std::vector<Method> reachable_methods = this->method_call_graph(main_method);

    auto is_reachable = [&reachable_methods](const Method &m) -> bool {
        if (m.method_name() == "<init>" || m.method_name() == "main") {
            return true;
        }

        for (const Method &o : reachable_methods) {
            if (m == o) {
                return true;
            }
        }
        return false;
    };

    for (ClassFile &cf : this->m_files) {
        std::vector<Method> to_remove;

        for (Method m : Method::all_from_classfile(cf)) {
            if (!is_reachable(m)) {
                to_remove.push_back(m);
            }
        }

        if (to_remove.empty()) {
            continue;
        }

        debug << "Found " << to_remove.size()
                  << " method(s) to remove from class " << cf->class_name()
                  << "\n";
        for (Method m : to_remove) {
            debug << "Removing " << m.format() << "...\n";
            cf = m.refresh(cf).with_this_method_removed();
        }
    }
}

void write_classfile_to_file(const ClassFile& cf, const fs::path& dest)
{
    std::ofstream fout(dest, std::ios::binary);
    const std::vector<u1> data = cf->serialize();
    fout.write(reinterpret_cast<const char *>(data.data()), data.size());
}

void Project::save(std::experimental::filesystem::path path) const
{
    for (ClassFile cf : this->m_files) {
        write_classfile_to_file(cf, path / (cf->class_name() + ".class"));
    }
}

void Project::save_in_place(std::vector<fs::path> filenames) const
{
    auto path_for_classfile = [&filenames] (const ClassFile& cf) -> fs::path
    {
        for (fs::path p : filenames) {
            if (cf->class_name() == p.stem()) {
                return p;
            }
        }
        assert (false);
    };

    for (ClassFile cf : this->m_files) {
        write_classfile_to_file(cf, path_for_classfile(cf));
    }
}

std::vector<ClassFile> Project::classfiles() const
{
    return this->m_files;
}

std::vector<Method> Project::sibling_methods(const Method& m) const
{
    auto method_is_sibling = [&] (const Method& o) -> bool
    {
        return m.method_type() == o.method_type()
            and m.method_name() == o.method_name()
            and class_is_descendant(o.class_file(), m.class_file());
    };

    std::vector<Method> ret;
    for (const ClassFile& cf : this->m_files) {
        for (const Method& o : Method::all_from_classfile(cf)) {
            if (method_is_sibling(o)) {
                ret.push_back(o);
            } else {
                debug << m.format() << " and " << o.format() << " are not sibling methods\n";
            }
        }
    }
    debug << "Found the following sibling methods for " << m.format() << ": ";
    for (const Method& m : ret) {
        debug << m.format() << "; ";
    }
    debug << "\n";
    return ret;
}

bool Project::class_is_descendant(const ClassFile& down, const ClassFile& up) const
{
    if (down == up) {
        return true;
    }

    std::vector<ClassFile> reachable = {down};
    auto direct_links = [this] (const ClassFile& cf) -> std::vector<ClassFile>
    {
        std::vector<ClassFile> ret;

        auto push_maybe = [&ret] (std::optional<ClassFile> maybe_cf) -> void
        {
            if (maybe_cf) {
                ret.push_back(maybe_cf.value());
            }
        };

        push_maybe(this->resolve_classfile(cf->super_class_name()));

        for (const std::string& i : cf->interface_names()) {
            push_maybe(this->resolve_classfile(i));
        }

        return ret;
    };
    auto is_reachable = [&reachable] (const ClassFile& cf) -> bool
    {
        for (const ClassFile& cf2 : reachable) {
            if (cf == cf2) {
                return true;
            }
        }
        return false;
    };
    for (size_t at = 0; at < reachable.size(); at++) {
        const ClassFile& cf = reachable[at];

        for (const ClassFile& next : direct_links(cf)) {
            if (is_reachable(next)) {
                continue;
            }
            reachable.push_back(next);
        }
    }
    if (is_reachable(up)) {
        debug << down->class_name() << " is a descendant of " << up->class_name() << "\n";
        return true;
    } else {
        debug << down->class_name() << " is NOT a descendant of " << up->class_name() << "\n";
        return false;
    }
}


std::vector<Method> Project::method_call_graph(const Method& m) const
{
    std::vector<Method> ret;
    auto add_method = [&ret, this] (const Method& m) -> void
    {
        for (const Method &o : this->sibling_methods(m)) {
            if (o.method_name() != "<init>") {
                ret.push_back(o);
            }
        }
    };
    add_method(m);
    size_t at = 0;

    auto was_visited = [&ret] (const Method& m) -> bool
    {
        for (const Method &o : ret) {
            if (o == m) {
                return true;
            }
        }
        return false;
    };

    while (at < ret.size()) {
        const Method& m = ret[at];
        at += 1;
        for (const Method& o : m.called_methods()) {
            if (!was_visited(o)) {
                add_method(o);
            }
        }
    }

    return ret;
}
