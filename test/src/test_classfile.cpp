#include <vector>
#include <experimental/filesystem>

#include "util.h"
#include "gtest/gtest.h"
#include "classfile.h"

namespace fs = std::experimental::filesystem;

std::vector<JavaProject> fixtures()
{
    fs::path fixtures = fs::path(__FILE__).parent_path().parent_path() / "fixtures";
    fs::path p0 = fixtures / "project0";

    JavaProject jp;
    for (auto &p : fs::directory_iterator(p0)) {
        if (p.path().extension() != ".class") {
            continue;
        }
        jp.class_files.push_back(p.path());
    }
    return {jp};
}

void expect_classfiles_equal(const ClassFile& cf1, const ClassFile& cf2)
{
#define EXPECT_FIELD_EQ(fieldname)\
    EXPECT_EQ(cf1.fieldname, cf2.fieldname);

    EXPECT_FIELD_EQ(magic);
    EXPECT_FIELD_EQ(minor_version);
    EXPECT_FIELD_EQ(major_version);
    EXPECT_FIELD_EQ(constant_pool_count);
    EXPECT_FIELD_EQ(constant_pool);
    EXPECT_FIELD_EQ(access_flags);
    EXPECT_FIELD_EQ(this_class);
    EXPECT_FIELD_EQ(super_class);
    EXPECT_FIELD_EQ(interface_count);
    EXPECT_FIELD_EQ(interfaces);
    EXPECT_FIELD_EQ(field_count);
    EXPECT_FIELD_EQ(fields);
    EXPECT_FIELD_EQ(method_count);
    EXPECT_FIELD_EQ(methods);
    EXPECT_FIELD_EQ(attribute_count);
    EXPECT_FIELD_EQ(attributes);

#undef EXPECT_FIELD_EQ
}

TEST(ClassFileTest, deserialize_and_serialize_are_inverses)
{
    for (JavaProject& jp : fixtures()) {
        for (std::string& file : jp.class_files) {
            std::vector<u1> data = read_entire_file(file);
            std::vector<u1> conv_data = ClassFile::deserialize(data).serialize();

            expect_classfiles_equal(ClassFile::deserialize(data),
                                    ClassFile::deserialize(conv_data));

            EXPECT_EQ(data, conv_data);
        }
    }
}
