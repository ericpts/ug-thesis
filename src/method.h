#pragma once

#include "classfile.h"
#include <memory>

// This represents a view into a method reference of a class file.
struct Method {
public:
    // Enum of the access flags for a given method.
    enum class FLAGS {
        ACC_ABSTRACT = 0x0400,
    };

  private:
    // The class file where this method belongs.
    ClassFile m_class_file;

    // The index of this method in the class file method table.
    int m_method_index;

    // Constructs the method from the ClassFile and the method index.
    Method(ClassFile file, int index);

  public:
    // After a class file has been changed, *this* method will still reference
    // the old one.
    // In order to resolve this method in the new class file, it has to search
    // for the new index
    // and create a new struct.
    Method refresh(const ClassFile &new_file) const;

    // Static constructors for a method struct.
    // They require a third redundant parameter in order to assert that they are
    // being called correctly.
    //
    // Constructs the method from the ClassFile which owns this method, and the
    // method info.
    //
    // This should be called as such:
    // Method::from_owner(file, index, file.methods[index]).
    static Method from_owner(const ClassFile &file, int index,
                             method_info info);

    // This method is referenced by `file` at the index `cp_index` of the
    // constant pool.
    // We first have to resolve the reference, and then instantiate the method.
    // This returns an optional, instead of the method itself, because the
    // refernce may point to a
    // library method, which we do not have access to.
    //
    // This should be called as such:
    // Method::from_symbolic_reference(file, index, file.constant_pool[index]).
    //
    // This method implicitly relies on the global project state, because the
    // referenced
    // method may not always be part of the current class.
    static std::optional<Method>
    from_symbolic_reference(const ClassFile &file, int cp_index, cp_info info);

    // Returns all the methods referenced in `file`.
    static std::vector<Method> all_from_classfile(const ClassFile &file);

    // Tries to retrieve the main method of `file`.
    static std::optional<Method> main_method(const ClassFile &file);

    // Returns the class file where this class belongs.
    const ClassFile &class_file() const;

    // Returns the code attribute struct corresponding to this method.
    // If this method is abstract, then it does not contain any code.
    std::optional<Code_attribute> code_attribute() const;

    // Returns the name of this method.
    std::string method_name() const;

    // Returns the type of this method.
    std::string method_type() const;

    // Format this method for human view with the class name, method name and
    // method type.
    // This can be used for method comparison if the methods belong to updated
    // class files,
    // as the string is independent of the method's index.
    std::string format() const;

    // Returns all the methods that *this* method calls directly.
    std::vector<Method> called_methods() const;

    // Removes *this* method from the original class file, returning a new one
    // with this method removed.
    ClassFile with_this_method_removed() const;

    // Returns whether this method is abstract.
    bool is_abstract() const;

    // This compares based based on class file and index, not on actual value.
    // In other words, updating the class file will cause methods to compre
    // unequal.
    bool operator==(const Method &o) const;
};
