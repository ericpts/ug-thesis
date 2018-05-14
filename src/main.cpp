#include <CLI/CLI.hpp>

#include <cassert>
#include <vector>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

#include "types.h"
#include "classfile.h"
#include "classparser.h"

int main(int argc, char** argv)
{
    std::ifstream fin(argv[1], std::ios::binary);
    std::vector<u1> data(
            (std::istream_iterator<u1>(fin)),
             std::istream_iterator<u1>());

    ClassFile file = ClassFile::deserialize(data);
    return 0;
}
