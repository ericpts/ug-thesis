#!/bin/bash

set -e

if ! javap I 2>/dev/null | grep "public abstract void foo()" &>/dev/null ; then
    echo "Could not find I::foo()"
    exit 1
fi

if ! javap "Main\$C" 2>/dev/null | grep "public void foo()" &>/dev/null ; then
    echo "Could not find Main::C::foo()"
    exit 1
fi

java Main
