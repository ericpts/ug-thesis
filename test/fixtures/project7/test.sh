#!/bin/bash

if ! cat thesis.log | grep "ERROR" &>/dev/null; then
    echo "The program should error when encountering reflection!"
    exit 1
fi
