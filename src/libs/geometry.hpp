#pragma once

#include <random>
#include "generic_types.hpp"

// ###############################################
// #tag Structs
// ###############################################

class Geometry
{
  public:
    // The orthographic projection matrix is typically defined by the left, right, bottom, top, near, and far
    // clipping planes. The matrix can be represented as follows:
    // | 2/(right-left)       0                0              -(right+left)/(right-left) |
    // | 0                   2/(top-bottom)    0              -(top+bottom)/(top-bottom) |
    // | 0                   0               -2/(far-near)    -(far+near)/(far-near)     |
    // | 0                   0                0               1                          |
    static Mat4 orthographic_projection(float left, float right, float top, float bottom)
    {
        Mat4 result = {};
        result.aw = -(right + left) / (right - left);
        result.bw = (top + bottom) / (top - bottom);
        result.cw = 0.0f; // Near Plane
        result[0][0] = 2.0f / (right - left);
        result[1][1] = 2.0f / (top - bottom);
        result[2][2] = 1.0f / (1.0f - 0.0f); // Far and Near
        result[3][3] = 1.0f;
        return result;
    }

    static bool point_in_rect(Vec2 point, Rect rect)
    {
        return (point.x >= rect.pos.x && point.x <= rect.pos.x + rect.size.x && point.y >= rect.pos.y && point.y <= rect.pos.y + rect.size.y);
    }

    static bool point_in_rect(Vec2 point, IRect rect)
    {
        return (point.x >= rect.pos.x && point.x <= rect.pos.x + rect.size.x && point.y >= rect.pos.y && point.y <= rect.pos.y + rect.size.y);
    }

    static bool isColliding(IRect a, IRect b)
    {
        return a.pos.x < b.pos.x + b.size.x && // Collision on Left of a and right of b
               a.pos.x + a.size.x > b.pos.x && // Collision on Right of a and left of b
               a.pos.y < b.pos.y + b.size.y && // Collision on Bottom of a and Top of b
               a.pos.y + a.size.y > b.pos.y;   // Collision on Top of a and Bottom of b
    }
};
