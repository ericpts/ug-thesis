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

TEST(ClassFileTest, deserialize_and_serialize_are_inverses)
{
    for (JavaProject& jp : fixtures()) {
        for (std::string& file : jp.class_files) {
            std::vector<u1> data = read_entire_file(file);
            std::vector<u1> conv_data = ClassFile::deserialize(data).serialize();
            EXPECT_EQ(data, conv_data);
        }
    }
}
