#pragma once

#include "classfile.h"

// This represents a view into a method reference of a class file.
struct Method {
  private:
    // The class file where this method was referenced from.
    const ClassFile *m_class_file;

    // The index of this method in the class file method table.
    int m_method_index;

  public:
    // Constructs the method from the ClassFile and the method index.
    Method(const ClassFile &file, int index);

    // Returns the class file which references this method.
    const ClassFile &reference_class_file() const;

    // Returns the class file which owns this method.
    const std::string owner_class_name() const;

    // Returns the code attribute struct corresponding to this method.
    Code_attribute code_attribute() const;

    // Returns the name of this method.
    std::string method_name() const;

    // Format this method for human view.
    std::string format() const;

    // Returns all the methods that *this* method calls.
    std::vector<Method> called_methods() const;
};
