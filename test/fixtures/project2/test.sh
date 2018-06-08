#!/bin/bash

set -e

if javap Other 2>/dev/null | grep "foo(" &>/dev/null ; then
    echo "Other still has foo!"
    exit 1
fi

java Main


