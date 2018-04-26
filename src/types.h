#pragma once

#include <cstdint>
#include <vector>

#include <arpa/inet.h>

using u1 = uint8_t;
using u2 = uint16_t;
using u4 = uint32_t;

template<typename T>
T to_host(T network);


template<>
u2 to_host(u2 network)
{
    return ntohs(network);
}

template<>
u4 to_host(u4 network)
{
    return ntohl(network);
}

struct Constant
{
    enum class Tag: u1 {
        utf8 = 1,
        i32 = 3,
        f32 = 4,
        i64 = 5,
        f64 = 6,
        cref = 7,
        sref = 8,
        fref = 9,
        mref = 10,
        iref = 11,
        ntdesc = 12,
        mhandle = 15,
        mtype = 16,
        idynamic = 18,
    };

    Tag tag;
    std::vector<u1> data;
};

// Each value in the interfaces array must be a valid index into the constant_pool table. The constant_pool entry at each value of interfaces[i], where 0 ≤ i < interfaces_count, must be tagged with Constant::Tag::cref representing an interface that is a direct superinterface of this class or interface type, in the left-to-right order given in the source for the type.
struct Interface
{
    u2 idx;
};

struct Field
{

};

struct Method
{

};

struct Attribute
{

};
