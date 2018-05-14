#include "classfile.h"
#include "classreader.h"
#include "classwriter.h"

ClassFile ClassFile::deserialize(const std::vector<uint8_t> &data)
{
    ClassReader reader(data);
    return reader.deserialize();
}

std::vector<uint8_t> ClassFile::serialize() const
{
    ClassWriter writer(*this);
    return writer.serialize();
}
