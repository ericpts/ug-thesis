#include <vector>
#include <experimental/filesystem>

#include "util.h"
#include "gtest/gtest.h"
#include "classfile.h"
#include "project.h"

namespace fs = std::experimental::filesystem;

std::vector<Project> fixtures()
{
    fs::path fixtures = fs::path(__FILE__).parent_path().parent_path() / "fixtures";

    int i = 0;
    auto next_project = [&i, &fixtures] () -> std::optional<fs::path>
    {
            fs::path p = (fixtures / ("project" + std::to_string(i)));
            i++;
            if (fs::exists(p)) {
                return p;
            }
            return {};
    };

    std::vector<Project> ret;

    while (std::optional<fs::path> maybe_path = next_project()) {
        fs::path path = maybe_path.value();
        std::vector<fs::path> classfiles;
        for (auto &p : fs::directory_iterator(path)) {
            if (p.path().extension() != ".class") {
                continue;
            }
            classfiles.push_back(p.path());
        }
        ret.push_back(Project{classfiles});
    }
    return ret;

}

void expect_classfiles_equal(const ClassFileImpl& cf1, const ClassFileImpl& cf2)
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
    for (Project& p : fixtures()) {
        for (const ClassFile& cf : p.classfiles()) {
            std::vector<u1> data = cf->serialize();
            std::vector<u1> conv_data = ClassFileImpl::deserialize(data).serialize();
            expect_classfiles_equal(ClassFileImpl::deserialize(data),
                                    ClassFileImpl::deserialize(conv_data));
            EXPECT_EQ(data, conv_data);
        }
    }
}

