#pragma once

#include "classfile.h"

// This represents a view into a method reference of a class file.
struct Method {
  private:
    // The class file where this method belongs.
    const ClassFile *m_class_file;

    // The index of this method in the class file method table.
    int m_method_index;

    // Constructs the method from the ClassFile and the method index.
    Method(const ClassFile &file, int index);

  public:
    // Static constructors for a method struct.
    // They require a third redundant parameter in order to assert that they are being called correctly.
    //
    // Constructs the method from the ClassFile which owns this method, and the method info.
    //
    // This should be called as such:
    // Method::from_owner(file, index, file.methods[index]).
    static Method from_owner(const ClassFile &file, int index, method_info info);

    // This method is referenced by `file` at the index `cp_index` of the constant pool.
    // We first have to resolve the reference, and then instantiate the method.
    //
    // This should be called as such:
    // Method::from_symbolic_reference(file, index, file.constant_pool[index]).
    static Method from_symbolic_reference(const ClassFile &file, int cp_index, cp_info info);

    // Returns all the methods referenced in `file`.
    static std::vector<Method> all_from_classfile(const ClassFile& file);

    // Tries to retrieve the main method of `file`.
    static std::optional<Method> main_method(const ClassFile& file);

    // Returns the class file where this class belongs.
    const ClassFile &class_file() const;

    // Returns the code attribute struct corresponding to this method.
    Code_attribute code_attribute() const;

    // Returns the name of this method.
    std::string method_name() const;

    // Format this method for human view.
    std::string format() const;

    // Returns all the methods that *this* method calls.
    std::vector<Method> called_methods() const;
};
