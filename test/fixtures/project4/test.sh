#!/bin/bash

set -e

if ! javap Other 2>/dev/null | grep "foo()" &>/dev/null ; then
    echo "Could not find Other::foo()"
    exit 1
fi

if ! javap "Main\$One" 2>/dev/null | grep "foo()" &>/dev/null ; then
    echo "Could not find Main::One::foo()"
    exit 1
fi

java Main


