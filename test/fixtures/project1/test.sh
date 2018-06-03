#!/bin/bash

set -e

if ! javap Main 2>/dev/null | grep "foo(" &>/dev/null ; then
    exit 1
fi

java Main
