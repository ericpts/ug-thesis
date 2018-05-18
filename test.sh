#!/bin/bash

set -e

./build.sh

pushd build
make test
popd

exe=$PWD/build/thesis
pushd test
make

pushd fixtures

for f in *; do
    $exe $f/*.class
done

popd # fixtures
popd # test
