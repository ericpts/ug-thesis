#!/bin/bash

set -e

mkdir -p build
pushd build
conan install ..
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
cmake --build .
