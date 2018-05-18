#!/usr/bin/env bash

function install_requisites() {
    sudo apt install\
        openjdk-11-jdk
}

function main() {
    install_requisites
}

main
