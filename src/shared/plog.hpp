#pragma once

#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>

#ifdef _WIN32
#define DEBUG_BREAK() __debugbreak()
#elif __linux__
#define DEBUG_BREAK() __builtin_debugtrap()
#elif __APPLE__
#define DEBUG_BREAK() __builtin_trap()
#endif

#define PLOG_ASSERT(x, msg)                                                                                                                                    \
    if (!(x)) {                                                                                                                                                \
        PLOG_FATAL << msg;                                                                                                                                     \
        DEBUG_BREAK();                                                                                                                                         \
        PLOG_FATAL << "Assertion Hit!";                                                                                                                        \
    }
