#ifdef ENGINE // Inside Game / Engine
#pragma once
#include "../libs/generic_types.hpp"
#else // Inside Shader
#define BIT(i) 1 << i
#endif

// #############################################################################
//                           Rendering Constants
// #############################################################################

int RENDERING_OPTION_NONE = 0;
int RENDERING_OPTION_FLIP_X = BIT(0);
int RENDERING_OPTION_FLIP_Y = BIT(1);
int RENDERING_OPTION_FONT = BIT(2);

int TRANSFORM_TYPE_NONE = 0;
int TRANSFORM_TYPE_SPRITE = BIT(0);
int TRANSFORM_TYPE_OUTLINE = BIT(1);
int TRANSFORM_TYPE_FILL = BIT(2);

// #############################################################################
//                           Rendering Structs
// #############################################################################

// Size and Alignment of basic types:
// bool => 1
// char => 1
// (unsigned) int => 4
// (unsigned) long long => 8
// (unsigned) float => 4
// (unsigned) double => 8
// (unsigned) long double => 16
// pointers (64-bit system) => 8

// Alignments of Structs in C++ are based on the largest member in the struct.
//

#ifdef ENGINE // Inside Game / Engine

// Transformation Object that holds information about what is being rendered
struct alignas(8) Transform {
    Vec2 pos;           // [8] The position in the world.
    Vec2 size;          // [8] The size the sprite should be rendered as.
    int renderOptions;  // [4] The options for rendering this object.
    int transformType;  // [4] The type of transformation this is.
    IVec2 spriteOffset; // [8] The offset (in pixels) in the texture file this a sprite is located.
    IVec2 spriteSize;   // [8] The size (in pixels) this sprite is in the texture file.
    int materialIndex;  // [4] The index of the material this object should use.
    int foo;            // [4] **Padding to ensure the struct is aligned to 8 bytes.
};

#else // Inside Shader

struct Transform {
    vec2 pos;
    vec2 size;
    int renderOptions;
    int transformType;
    ivec2 spriteOffset;
    ivec2 spriteSize;
    int materialIndex;
    int foo;
};

#endif
