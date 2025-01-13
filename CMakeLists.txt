cmake_minimum_required(VERSION 3.10)

# Project name and version
project(MyFirstGame VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(third_party third_party/opengl)

# Compiler warnings
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # MSVC specific options
    add_compile_options(/W3)
else()
    # GCC/Clang specific options
    add_compile_options(-Wno-writable-strings -Wno-format-security -Wno-deprecated-declarations)
endif()

# Libraries
set(LIBS user32 opengl32 gdi32)

# Executable target
add_executable(game src/main.cpp) # Executable
target_compile_definitions(game PRIVATE ENGINE)
target_link_libraries(game ${LIBS}) # Link libraries
set_target_properties(game PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/builds) # Output Director

# Shared library target
add_library(game_shared SHARED src/game.cpp) # Shared library
target_compile_definitions(game_shared PRIVATE ENGINE)
target_link_libraries(game_shared ${LIBS}) # Link libraries
set_target_properties(game_shared PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/builds) # Output Directory
set_target_properties(game_shared PROPERTIES OUTPUT_NAME "game") # Rename the shared library to game.dll

## ============================= ##
## ========== Assets =========== ##
## ============================= ##

# Copy assets to the build directory
set(ASSETS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/assets")
set(ASSETS_DEST_DIR "${CMAKE_BINARY_DIR}/builds/assets")
file(COPY ${ASSETS_SOURCE_DIR} DESTINATION ${ASSETS_DEST_DIR})

## ============================= ##
## ======= Hot Reloading ======= ##
## ============================= ##

# Custom command to copy the shared library to the desired location
add_custom_command(TARGET game_shared POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:game_shared> ${CMAKE_BINARY_DIR}/builds/game_load.dll
)
add_custom_command(TARGET game_shared PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E rm -f ${CMAKE_BINARY_DIR}/builds/Debug/game.dll
)
add_custom_command(TARGET game_shared PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E rm -f ${CMAKE_BINARY_DIR}/builds/Debug/game.pdb
)

# Custom target to build and copy the shared library
add_custom_target(copy_game_shared ALL DEPENDS game_shared)

# file (REMOVE
#     ${CMAKE_BINARY_DIR}/dist/*.asm.js
#     )