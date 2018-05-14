#!/bin/bash

set -e

mkdir -p build
pushd build

cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
cmake --build .

popd
