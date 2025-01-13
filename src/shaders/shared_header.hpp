#ifdef ENGINE // Inside Game / Engine
#pragma once

#include "../libs/libs.hpp"
#define vec2 Vec2
#define ivec2 IVec2
#define vec4 Vec4

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

// Transformation Object that holds information about what is being rendered
struct Transform {
    vec2 pos;  // The position in the world
    vec2 size; // The size the sprite should be rendered as

    int type;

    ivec2 spriteOffset; // The offset (in pixels) in the texture file this a sprite is located
    ivec2 spriteSize;   // The size (in pixels) this sprite is

    // int renderOptions;
    // int materialIdx;
    // float layer;
    // int padding;
};

// struct Material {
//     // Operator inside the Engine to compare materials
// #ifdef ENGINE
//     Color color = Colors::White;
//     bool operator==(Material other)
//     {
//         return color == other.color;
//     }
// #else
//     vec4 color;
// #endif
// };