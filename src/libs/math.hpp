#pragma once

#include <random>
#include "generic_types.hpp"

// ###############################################
// #tag Functions
// ###############################################

class Math
{
  public:
    // See: https://stackoverflow.com/questions/7114043/random-number-generation-in-c11-how-to-generate-how-does-it-work
    static int random(int minValue, int maxValue)
    {
        std::random_device dev; // Seed
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist6(minValue, maxValue);
        return dist6(rng);
    }

    static float approach(float current, float target, float deltaTime)
    {
        float difference = target - current;
        if (difference > deltaTime) {
            return current + deltaTime;
        } else if (difference < -deltaTime) {
            return current - deltaTime;
        } else {
            return target;
        }
    }

    // Given a straight line from point a to b, find the t% through it.
    // Examples: t = 0 is start, t = 1 is end, t = 0.5 is middle.
    static float lerp(float a, float b, float t)
    {
        return a + (b - a) * t;
    }

    static Vec2 lerp(Vec2 a, Vec2 b, float t)
    {
        Vec2 result;
        result.x = lerp(a.x, b.x, t);
        result.y = lerp(a.y, b.y, t);
        return result;
    }

    static IVec2 lerp(IVec2 a, IVec2 b, float t)
    {
        IVec2 result;
        result.x = (int)floorf(lerp((float)a.x, (float)b.x, t));
        result.y = (int)floorf(lerp((float)a.y, (float)b.y, t));
        return result;
    }

    template <typename T> static int sign(T val)
    {
        return (T(0) < val) - (val < T(0));
    }
};