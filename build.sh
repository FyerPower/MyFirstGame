#!/bin/bash

libs="-luser32 -lopengl32 -lgdi32"
warnings="-Wno-writable-strings -Wno-format-security -Wno-deprecated-declarations"
includes="-Ithird_party -Ithird_party/opengl"

clang++ -g src/main.cpp -o builds/game.exe $libs $warnings $includes

timestamp=$(date +%s)
rm -f builds/game_* # Remove old game_* files
clang++ -g src/game.cpp -o builds/game_$timestamp.dll -shared $warnings
mv builds/game_$timestamp.dll builds/game.dll