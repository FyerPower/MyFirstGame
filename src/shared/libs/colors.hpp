#pragma once

#include <cstdint>

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    Color() : r(0), g(0), b(0), a(255)
    {}

    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255) : r(red), g(green), b(blue), a(alpha)
    {}

    bool operator==(const Color& other) const
    {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }
};

// Predefined colors
namespace Colors {
const Color Red(255, 0, 0);
const Color Green(0, 255, 0);
const Color Blue(0, 0, 255);
const Color White(255, 255, 255);
const Color Black(0, 0, 0);
const Color Yellow(255, 255, 0);
const Color Cyan(0, 255, 255);
const Color Magenta(255, 0, 255);
const Color Transparent(0, 0, 0, 0);
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