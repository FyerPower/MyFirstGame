#pragma once

// ###############################################
//                     Structs
// ###############################################

struct IVec2 {
    int x;
    int y;

    IVec2 operator-(IVec2 other)
    {
        return {x - other.x, y - other.y};
    }
};

struct Vec2 {
    float x;
    float y;

    Vec2 operator/(float scalar)
    {
        return {x / scalar, y / scalar};
    }
    Vec2 operator-(Vec2 other)
    {
        return {x - other.x, y - other.y};
    }
};

struct Vec4 {
    union {
        float values[4];
        struct {
            float x;
            float y;
            float z;
            float w;
        };
        struct {
            float r;
            float g;
            float b;
            float a;
        };
    };

    float& operator[](int idx)
    {
        return values[idx];
    }
};

struct Mat4 {
    union {
        Vec4 values[4];
        struct {
            float ax;
            float bx;
            float cx;
            float dx;
            float ay;
            float by;
            float cy;
            float dy;
            float az;
            float bz;
            float cz;
            float dz;

            float aw;
            float bw;
            float cw;
            float dw;
        };
    };

    Vec4& operator[](int col)
    {
        return values[col];
    }
};

struct OrthographicCamera2D {
    float zoom = 1.0f;
    Vec2 dimensions;
    Vec2 position;
};

// ###############################################
//                     Functions
// ###############################################

Vec2 vec_2(IVec2 v)
{
    return Vec2{(float)v.x, (float)v.y};
}

Mat4 orthographic_projection(float left, float right, float top, float bottom)
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