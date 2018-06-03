#!/bin/bash

set -e

output=$(javap Main 2>/dev/null)

for i in $(seq 1 104); do
    if ! echo "$output" | grep "f_$i(" &>/dev/null ; then
        echo "Could not find f_$i()"
        exit 1
    fi
done

java Main
