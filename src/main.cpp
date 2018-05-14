#include <CLI11.hpp>

#include "classfile.h"
#include "classreader.h"
#include "types.h"
#include "util.h"

int main(int argc, char **argv)
{
    std::string filename = argv[1];
    ClassFile file = ClassFile::deserialize(read_entire_file(filename));
    return 0;
}
