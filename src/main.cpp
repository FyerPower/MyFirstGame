#include "input.hpp"
#include "libs/logger.hpp"

#define APIENTRY
#define GL_GLEXT_PROTOTYPES
#include "glcorearb.h"

// ###############################################
//                     Platforms
// ###############################################

#include "platform/platform.hpp"

#ifdef _WIN32
#include "platform/platform_windows.cpp"
#elif __linux__
#include "platform/platform_linux.cpp"
#elif __APPLE__
#include "platform/platform_mac.cpp"
#endif

#include "gl_renderer.cpp"

// ###############################################
//                     Globals
// ###############################################

int main()
{
    BumpAllocator transientStorage = make_bump_allocator(MB(50));

    // Create the Window
    input.screenSizeX = 1280;
    input.screenSizeY = 720;
    platform_create_window(input.screenSizeX, input.screenSizeY, "My Game");

    // Initialize OpenGL
    gl_init(&transientStorage);

    FP_LOG("Window Created");

    // Main Game Loop
    while (isRunning) {
        // Update the Window
        platform_update_window();

        // Update OpenGL
        gl_render();

        // Swap Buffers
        platform_swap_buffers();

        // FP_LOG("Hello");
        // FP_WARN("Hello");
        // FP_ERROR("Hello");
        // FP_ASSERT(true, "Assertion Hit");
    }

    // Return 0 when game loop is done (exitting appliation)
    return 0;
}