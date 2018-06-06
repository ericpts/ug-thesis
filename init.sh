#!/usr/bin/env bash

function install_latex_prerequisites() {
    sudo apt install xzdec texlive
    tlmgr init-usertree
    tlmgr install esvect
}

function install_prerequisites() {
    install_latex_prerequisites

    sudo apt install\
        openjdk-11-jdk

    pip3 install -r requirements.txt
}

function main() {
    install_prerequisites
}

main
