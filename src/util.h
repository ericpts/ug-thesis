#pragma once
#include <experimental/filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "types.h"

std::vector<u1> read_entire_file(const std::string &filename);

// Whether to enable debugging output.
const bool DO_DEBUG = true;

struct DebugOstream {
    template <typename T> DebugOstream &operator<<(const T &t)
    {
        if (DO_DEBUG) {
            std::cerr << t;
        }
        return *this;
    }
};

extern DebugOstream debug;
