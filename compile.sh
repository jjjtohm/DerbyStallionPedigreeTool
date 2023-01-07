#!/bin/bash

g++ src/search/PedigreeTool.cpp test/main.cpp\
    -o pedtool -Isrc -I. -std=c++17 -O3 -Wall -Wextra -DNDEBUG
