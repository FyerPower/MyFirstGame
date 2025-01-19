#pragma once

// ###############################################
// #tag Defines
// ###############################################

#define b8 char;
#define ArraySize(x) (sizeof((x)) / sizeof((x)[0]))

#define BIT(x) 1 << (x)
#define KB(x) ((unsigned long long)1024 * x)
#define MB(x) ((unsigned long long)1024 * KB(x))
#define GB(x) ((unsigned long long)1024 * MB(x))

// ###############################################
// #tag Struct
// ###############################################

struct IVec2 {
    int x;
    int y;

    IVec2 operator+(IVec2 other)
    {
        return {x + other.x, y + other.y};
    }

    IVec2& operator+=(int value)
    {
        x += value;
        y += value;
        return *this;
    }

    IVec2 operator-(IVec2 other)
    {
        return {x - other.x, y - other.y};
    }

    IVec2& operator-=(int value)
    {
        x -= value;
        y -= value;
        return *this;
    }

    IVec2 operator/(float scalar)
    {
        return {(int)(x / scalar), (int)(y / scalar)};
    }

    IVec2 operator/(int scalar)
    {
        return {x / scalar, y / scalar};
    }

    bool operator==(IVec2 other)
    {
        return x == other.x && y == other.y;
    }
};

struct Vec2 {
    float x;
    float y;

    Vec2 operator/(float scalar)
    {
        return {x / scalar, y / scalar};
    }

    Vec2 operator/(Vec2 other)
    {
        return {x / other.x, y / other.y};
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

    bool operator==(Vec4 other)
    {
        return x == other.x && y == other.y && z == other.z && w == other.w;
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

struct Rect {
    Vec2 pos;
    Vec2 size;
};

struct IRect {
    IVec2 pos;
    IVec2 size;

    IRect operator+(IVec2 startingPos)
    {
        return {pos + startingPos, size};
    }

    IRect operator-(IVec2 startingPos)
    {
        return {pos - startingPos, size};
    }

    bool operator==(IRect other)
    {
        return pos == other.pos && size == other.size;
    }
};

// ###############################################
// #tag Functions
// ###############################################

Vec2 vec_2(IVec2 v)
{
    return Vec2{(float)v.x, (float)v.y};
}