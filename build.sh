#!/bin/bash

libs="-luser32 -lopengl32 -lgdi32"
warnings="-Wno-writable-strings -Wno-format-security -Wno-deprecated-declarations"
includes="-Ithird_party -Ithird_party/opengl"

clang++ -g src/main.cpp -o game.exe $libs $warnings $includes

timestamp=$(date +%s)
rm -f game_* # Remove old game_* files
clang++ -g src/game.cpp -o game_$timestamp.dll -shared $warnings
mv game_$timestamp.dll game.dll