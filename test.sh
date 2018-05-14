#!/bin/bash

set -e

./build.sh

pushd build
make test
popd

exe=$PWD/build/bin/thesis
pushd test
make

for f in $(find . -type f -name "*\.class"); do
    echo "Running test $f"
    $exe $f
done

popd
