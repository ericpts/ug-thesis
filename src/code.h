#pragma once
#include "bytesparser.h"

// This enum contains a subset of the JVM instructions:
// * those we are about - the method invokation ones
// * and the ones we have to read more bytes in order to skip over.
enum class Instr {
    // These are the code instructions we are interested in processing.
    invokedynamic = 0xba,
    invokeinterface = 0xb9,
    invokespecial = 0xb7,
    invokestatic = 0xb8,
    invokevirtual = 0xb6,

    aload = 0x19, // 1
    anewarray = 0xbd, // 2
    astore = 0x3a, // 1
    bipush = 0x10, // 1
    checkcast = 0xc0, // 2
    dload = 0x18, // 1
    dstore = 0x39, // 1
    fload = 0x17, // 1
    fstore = 0x38, // 1
    getfield = 0xb4, // 2
    getstatic = 0xb2, // 2
    goto_ = 0xa7, // 2
    goto_w = 0xc8, // 4
    if_acmpeq = 0xa5, // 2
    if_acmpne = 0xa6, // 2
    if_icmpeq = 0x9f, // 2
    if_icmpne = 0xa0, // 2
    if_icmplt = 0xa1, // 2
    if_icmpge = 0xa2, // 2
    if_icmpgt = 0xa3, // 2
    if_icmple = 0xa4, // 2
    ifeq = 0x99, // 2
    ifne = 0x9a, // 2
    iflt = 0x9b, // 2
    ifge = 0x9c, // 2
    ifgt = 0x9d, // 2
    ifle = 0x9e, // 2
    ifnonnull = 0xc7, // 2
    ifnull = 0xc6, // 2
    iinc = 0x84, // 2
    iload = 0x15, // 1
    instanceof = 0xc1, // 2
    istore = 0x36, // 1
    jsr = 0xa8, // 2
    jsr_w = 0xc9, // 4
    ldc = 0x12, // 1
    ldc_w = 0x13, // 2
    ldc2_w = 0x14, // 2
    lload = 0x16, // 1
    lookupswitch = 0xab, // VARIABLE!
    lstore = 0x37, // 1
    multianewarray = 0xc5, // 3
    new_ = 0xbb, // 2
    newarray = 0xbc, // 1
    putfield = 0xb5, // 2
    putstatic = 0xb3, // 2
    ret = 0xa9, // 1
    sipush = 0x11, // 2
    tableswitch = 0xaa, // VARIABLE!
    wide = 0xc4, // VARIABLE!
};

// In case the instruction was not a method invocation,
// we have to skip some bytes, corresponding to the instruction length.
void skip_instruction(Instr i, BytesParser* bp);
