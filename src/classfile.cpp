#include "classfile.h"
#include "classreader.h"

ClassFile ClassFile::deserialize(const std::vector<uint8_t> &data)
{
    ClassReader parser(data);
    return parser.deserialize();
}

std::vector<uint8_t> ClassFile::serialize() const
{
    std::vector<uint8_t> ret;
    return ret;
}
