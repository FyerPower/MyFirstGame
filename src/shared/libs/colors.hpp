#pragma once

#include <cstdint>

struct Color {
    float r;
    float g;
    float b;
    float a;

    Color() : r(0.0), g(0.0), b(0.0), a(1.0)
    {}

    Color(float red, float green, float blue, float alpha = 255) : r(red), g(green), b(blue), a(alpha)
    {}

    bool operator==(const Color& other) const
    {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }
};

// Predefined colors
namespace Colors {
const Color Red(1.0, 0.0, 0.0);
const Color Green(0.0, 1.0, 0.0);
const Color Blue(0.0, 0.0, 1.0);
const Color White(1.0, 1.0, 1.0);
const Color Black(0.0, 0.0, 0.0);
const Color Yellow(1.0, 1.0, 0.0);
const Color Cyan(0.0, 1.0, 1.0);
const Color Magenta(1.0, 0.0, 1.0);
const Color Transparent(0.0, 0.0, 0.0, 0.0);
} // namespace Colors

/*
 *    Example Usage:
 *
 *    void printColor(const Color& color) {
 *        std::cout << "Color(R: " << static_cast<int>(color.r)
 *                  << ", G: " << static_cast<int>(color.g)
 *                  << ", B: " << static_cast<int>(color.b)
 *                  << ", A: " << static_cast<int>(color.a) << ")\n";
 *    }
 *
 *    int main() {
 *        Color myColor = Colors::Red;
 *        printColor(myColor);
 *
 *        Color customColor(128, 64, 32);
 *        printColor(customColor);
 *
 *        return 0;
 *    }
 */