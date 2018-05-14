#include "classfile.h"
#include "classparser.h"

ClassFile ClassFile::deserialize(const std::vector<uint8_t> &data)
{
    ClassParser parser(data);
    return parser.parse_class();
}

std::vector<uint8_t> ClassFile::serialize() const
{
    assert(false);
}
