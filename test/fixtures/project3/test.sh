#!/bin/bash

for i in $(seq 1 104); do
    if ! javap out.Main 2>/dev/null | grep "f_$i(" &>/dev/null ; then
        echo "Could not find f_$i()"
        exit 1
    fi
done


