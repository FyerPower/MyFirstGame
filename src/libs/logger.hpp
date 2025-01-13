#pragma once

// ###############################################
// #tag Includes
// ###############################################

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

// namespace TextColor {
// const char* Black = "\x1b[30m";
// const char* Red = "\x1b[31m";
// const char* Green = "\x1b[32m";
// const char* Yellow = "\x1b[33m";
// const char* Blue = "\x1b[34m";
// const char* Magenta = "\x1b[35m";
// const char* Cyan = "\x1b[36m";
// const char* White = "\x1b[37m";
// const char* BrightBlack = "\x1b[90m";
// const char* BrightRed = "\x1b[91m";
// const char* BrightGreen = "\x1b[92m";
// const char* BrightYellow = "\x1b[93m";
// const char* BrightBlue = "\x1b[94m";
// const char* BrightMagenta = "\x1b[95m";
// const char* BrightCyan = "\x1b[96m";
// const char* BrightWhite = "\x1b[97m";
// const char* Reset = "\x1b[0m";
// }; // namespace TextColor

// namespace Logger {
// template <typename... Args> void log(const char* msg, Args&&... args)
// {
//     // std::string formatBuffer = std::format("LOG   | {} {} {}", TextColor::Green, msg, TextColor::Reset);
//     // std::string textBuffer = std::format(formatBuffer, args...);
//     // std::puts(textBuffer.c_str());

//     char formatBuffer[8192] = {};
//     sprintf_s(formatBuffer, sizeof(formatBuffer), "LOG   | %s %s %s", TextColor::Green, msg, TextColor::Reset);

//     char textBuffer[8192] = {};
//     sprintf_s(textBuffer, sizeof(textBuffer), formatBuffer, args...);

//     puts(textBuffer);
// }

// template <typename... Args> void warn(const char* msg, Args&&... args)
// {
//     char formatBuffer[8192] = {};
//     sprintf_s(formatBuffer, sizeof(formatBuffer), "WARN  | %s %s %s", TextColor::Yellow, msg, TextColor::Reset);

//     char textBuffer[8192] = {};
//     sprintf_s(textBuffer, sizeof(textBuffer), formatBuffer, args...);

//     puts(textBuffer);
// }

// template <typename... Args> void error(const char* msg, Args&&... args)
// {
//     char formatBuffer[8192] = {};
//     sprintf_s(formatBuffer, sizeof(formatBuffer), "ERROR | %s %s %s", TextColor::Red, msg, TextColor::Reset);

//     char textBuffer[8192] = {};
//     sprintf_s(textBuffer, sizeof(textBuffer), formatBuffer, args...);

//     puts(textBuffer);
// }

// template <typename... Args> void asssert(bool x, const char* msg, Args&&... args)
// {
//     if (!x) {
//         char formatBuffer[8192] = {};
//         sprintf_s(formatBuffer, sizeof(formatBuffer), "ERROR | %s %s %s", TextColor::Red, msg, TextColor::Reset);

//         char textBuffer[8192] = {};
//         sprintf_s(textBuffer, sizeof(textBuffer), formatBuffer, args...);

//         puts(textBuffer);

//         //
//         // error(msg, std::forward<Args>(args)...);
//         DEBUG_BREAK();
//         // error("Assertion Hit", std::forward<Args>(args)...);
//     }
// }

// }; // namespace Logger

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

template <typename... Args> void FP_LOG(const char* msg, Args&&... args)
{
    // std::string formatBuffer = std::format("LOG   | {} {} {}", TextColor::Green, msg, TextColor::Reset);
    // std::string textBuffer = std::format(formatBuffer, args...);
    // std::puts(textBuffer.c_str());

    char formatBuffer[8192] = {};
    sprintf_s(formatBuffer, sizeof(formatBuffer), "LOG   | %s %s %s", TextColor::Green, msg, TextColor::Reset);

    char textBuffer[8192] = {};
    sprintf_s(textBuffer, sizeof(textBuffer), formatBuffer, args...);

    puts(textBuffer);
}

template <typename... Args> void FP_WARN(const char* msg, Args&&... args)
{
    char formatBuffer[8192] = {};
    sprintf_s(formatBuffer, sizeof(formatBuffer), "WARN  | %s %s %s", TextColor::Yellow, msg, TextColor::Reset);

    char textBuffer[8192] = {};
    sprintf_s(textBuffer, sizeof(textBuffer), formatBuffer, args...);

    puts(textBuffer);
}

template <typename... Args> void FP_ERROR(const char* msg, Args&&... args)
{
    char formatBuffer[8192] = {};
    sprintf_s(formatBuffer, sizeof(formatBuffer), "ERROR | %s %s %s", TextColor::Red, msg, TextColor::Reset);

    char textBuffer[8192] = {};
    sprintf_s(textBuffer, sizeof(textBuffer), formatBuffer, args...);

    puts(textBuffer);
}

template <typename... Args> void FP_ASSERT(bool x, const char* msg, Args&&... args)
{
    if (!x) {
        char formatBuffer[8192] = {};
        sprintf_s(formatBuffer, sizeof(formatBuffer), "ERROR | %s %s %s", TextColor::Red, msg, TextColor::Reset);

        char textBuffer[8192] = {};
        sprintf_s(textBuffer, sizeof(textBuffer), formatBuffer, args...);

        puts(textBuffer);

        //
        // error(msg, std::forward<Args>(args)...);
        DEBUG_BREAK();
        // error("Assertion Hit", std::forward<Args>(args)...);
    }
}

// enum TextColor {
//     TEXT_COLOR_BLACK,
//     TEXT_COLOR_RED,
//     TEXT_COLOR_GREEN,
//     TEXT_COLOR_YELLOW,
//     TEXT_COLOR_BLUE,
//     TEXT_COLOR_MAGENTA,
//     TEXT_COLOR_CYAN,
//     TEXT_COLOR_WHITE,
//     TEXT_COLOR_BRIGHT_BLACK,
//     TEXT_COLOR_BRIGHT_RED,
//     TEXT_COLOR_BRIGHT_GREEN,
//     TEXT_COLOR_BRIGHT_YELLOW,
//     TEXT_COLOR_BRIGHT_BLUE,
//     TEXT_COLOR_BRIGHT_MAGENTA,
//     TEXT_COLOR_BRIGHT_CYAN,
//     TEXT_COLOR_BRIGHT_WHITE,
//     TEXT_COLOR_COUNT
// };

// template <typename... Args> void _log(char* prefix, char* msg, TextColor textColor, Args... args)
// {
//     static char* TextColorTable[TEXT_COLOR_COUNT] = {
//         "\x1b[30m", // TEXT_COLOR_BLACK,
//         "\x1b[31m", // TEXT_COLOR_RED,
//         "\x1b[32m", // TEXT_COLOR_GREEN,
//         "\x1b[33m", // TEXT_COLOR_YELLOW,
//         "\x1b[34m", // TEXT_COLOR_BLUE,
//         "\x1b[35m", // TEXT_COLOR_MAGENTA,
//         "\x1b[36m", // TEXT_COLOR_CYAN,
//         "\x1b[37m", // TEXT_COLOR_WHITE,
//         "\x1b[90m", // TEXT_COLOR_BRIGHT_BLACK,
//         "\x1b[91m", // TEXT_COLOR_BRIGHT_RED,
//         "\x1b[92m", // TEXT_COLOR_BRIGHT_GREEN,
//         "\x1b[93m", // TEXT_COLOR_BRIGHT_YELLOW,
//         "\x1b[94m", // TEXT_COLOR_BRIGHT_BLUE,
//         "\x1b[95m", // TEXT_COLOR_BRIGHT_MAGENTA,
//         "\x1b[96m", // TEXT_COLOR_BRIGHT_CYAN,
//         "\x1b[97m", // TEXT_COLOR_BRIGHT_WHITE,
//     };

//     char formatBuffer[8192] = {};
//     sprintf(formatBuffer, "%s %s %s \033[0m", TextColorTable[textColor], prefix, msg);

//     char textBuffer[8192] = {};
//     sprintf(textBuffer, formatBuffer, args...);

//     puts(textBuffer);
// }

// #define FP_LOG(msg, ...) _log("LOG:   ", msg, TEXT_COLOR_GREEN, ##__VA_ARGS__);
// #define FP_WARN(msg, ...) _log("WARN:  ", msg, TEXT_COLOR_YELLOW, ##__VA_ARGS__);
// #define FP_ERROR(msg, ...) _log("ERROR: ", msg, TEXT_COLOR_RED, ##__VA_ARGS__);
// #define FP_ASSERT(x, msg, ...) \
//     { \
//         if (!x) { \
//             FP_ERROR(msg, ##__VA_ARGS__); \
//             DEBUG_BREAK(); \
//             FP_ERROR("Assertion Hit", ##__VA_ARGS__); \
//         } \
//     }
