#!/bin/bash

if javap out.Other 2>/dev/null | grep "foo(" &>/dev/null ; then
    exit 1
fi


