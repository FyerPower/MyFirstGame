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
    Vec2 pos;           // The position in the world.
    Vec2 size;          // The size the sprite should be rendered as.
    int renderOptions;  // The options for rendering this object.
    int transformType;  // The type of transformation this is.
    IVec2 spriteOffset; // The offset (in pixels) in the texture file this a sprite is located.
    IVec2 spriteSize;   // The size (in pixels) this sprite is in the texture file.
};

#else // Inside Shader

struct Transform {
    vec2 pos;
    vec2 size;
    int renderOptions;
    int transformType;
    ivec2 spriteOffset;
    ivec2 spriteSize;
};

#endif
