#!/bin/sh

antlr4 -Dlanguage=Cpp -no-listener -no-visitor -package asgard::code -o grammar/asgard/code -Xexact-output-dir Asgard.g4

