#!/bin/bash

if ! javap out.Main 2>/dev/null | grep "foo(" &>/dev/null ; then
    exit 1
fi
