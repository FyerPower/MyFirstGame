#!/bin/bash

libs="-luser32 -lopengl32 -lgdi32"
warnings="-Wno-writable-strings -Wno-format-security -Wno-deprecated-declarations"
includes="-Ithird_party"

clang++ -g src/main.cpp -ogame.exe $libs $warnings $includes