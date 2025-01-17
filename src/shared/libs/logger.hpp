#pragma once

// ###############################################
// #tag Includes
// ###############################################

#include <format>
#include <iostream>
#include <string>

#include <stdio.h>

// ###############################################
// #tag Defines
// ###############################################

#ifdef _WIN32
#define DEBUG_BREAK() __debugbreak()
#define EXPORT_FN __declspec(dllexport)
#elif __linux__
#define DEBUG_BREAK() __builtin_debugtrap()
#define EXPORT_FN
#elif __APPLE__
#define DEBUG_BREAK() __builtin_trap()
#define EXPORT_FN
#endif

// ###############################################
// #tag Functions
// ###############################################

namespace TextColor {
const char* Black = "\x1b[30m";
const char* Red = "\x1b[31m";
const char* Green = "\x1b[32m";
const char* Yellow = "\x1b[33m";
const char* Blue = "\x1b[34m";
const char* Magenta = "\x1b[35m";
const char* Cyan = "\x1b[36m";
const char* White = "\x1b[37m";
const char* BrightBlack = "\x1b[90m";
const char* BrightRed = "\x1b[91m";
const char* BrightGreen = "\x1b[92m";
const char* BrightYellow = "\x1b[93m";
const char* BrightBlue = "\x1b[94m";
const char* BrightMagenta = "\x1b[95m";
const char* BrightCyan = "\x1b[96m";
const char* BrightWhite = "\x1b[97m";
const char* Reset = "\x1b[0m";
}; // namespace TextColor

namespace Logger {
template <typename... Args> void log(const char* msg, Args&&... args)
{
    char formatBuffer[8192] = {};
    sprintf_s(formatBuffer, sizeof(formatBuffer), "LOG  | %s %s %s", TextColor::Green, msg, TextColor::Reset);

    char textBuffer[8192] = {};
    sprintf_s(textBuffer, sizeof(textBuffer), formatBuffer, args...);

    puts(textBuffer);
}

template <typename... Args> void warn(const char* msg, Args&&... args)
{
    char formatBuffer[8192] = {};
    sprintf_s(formatBuffer, sizeof(formatBuffer), "WARN  | %s %s %s", TextColor::Yellow, msg, TextColor::Reset);

    char textBuffer[8192] = {};
    sprintf_s(textBuffer, sizeof(textBuffer), formatBuffer, args...);

    puts(textBuffer);
}

template <typename... Args> void error(const char* msg, Args&&... args)
{
    char formatBuffer[8192] = {};
    sprintf_s(formatBuffer, sizeof(formatBuffer), "ERROR | %s %s %s", TextColor::Red, msg, TextColor::Reset);

    char textBuffer[8192] = {};
    sprintf_s(textBuffer, sizeof(textBuffer), formatBuffer, args...);

    puts(textBuffer);
}

template <typename... Args> void asssert(bool x, const char* msg, Args&&... args)
{
    if (!x) {
        char formatBuffer[8192] = {};
        sprintf_s(formatBuffer, sizeof(formatBuffer), "ERROR | %s %s %s", TextColor::Red, msg, TextColor::Reset);

        char textBuffer[8192] = {};
        sprintf_s(textBuffer, sizeof(textBuffer), formatBuffer, args...);

        puts(textBuffer);
        DEBUG_BREAK();
        puts(textBuffer);
    }
}
} // namespace Logger