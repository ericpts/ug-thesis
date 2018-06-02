#!/usr/bin/env bash

function install_requisites() {
    sudo apt install\
        openjdk-11-jdk

    pip3 install -r requirements.txt
}

function main() {
    install_requisites
}

main
