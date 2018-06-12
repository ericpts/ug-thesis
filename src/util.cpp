#include "util.h"
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

std::vector<u1> read_entire_file(const std::string &filename)
{
    std::ifstream fin(filename, std::ios::binary);
    std::vector<u1> data((std::istreambuf_iterator<char>(fin)),
                         std::istreambuf_iterator<char>());
    return data;
}

DebugOstream debug;
