#ifdef ENGINE // Inside Game / Engine
#pragma once
#include "../libs/colors.hpp"
#else // Inside Shader
#endif

#ifdef ENGINE // Inside Game / Engine

// Transformation Object that holds information about what is being rendered
struct alignas(16) Material {
    Color color = Colors::White;

    bool operator==(Material other)
    {
        return color == other.color;
    }
};

#else // Inside Shader

struct Material {
    vec4 color;
};

#endif