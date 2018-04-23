#!/bin/bash

set -e

mkdir -p build
pushd build
conan install ..
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
cmake --build .

popd
ln -sf build/conan_ycm_extra_conf.py .ycm_extra_conf.py
ln -sf build/conan_ycm_flags.json .
