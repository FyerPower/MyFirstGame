#include "libs/libs.hpp"
#include "input.hpp"
#include "game.cpp"

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
    // Create 50MB of memory
    BumpAllocator transientStorage = make_bump_allocator(MB(50));

    // Create the Window
    input.screenSizeX = 1280;
    input.screenSizeY = 720;
    platform_create_window(input.screenSizeX, input.screenSizeY, "My Game");
    FP_LOG("Window Created");

    // Initialize OpenGL
    gl_init(&transientStorage);
    FP_LOG("Initialized OpenGL");

    // Main Game Loop
    FP_LOG("======= Game Loop Begin =======");
    while (isRunning) {
        // Update the Window
        platform_update_window();
        FP_LOG("Initialized Transient Storage");

        // Update Game
        update_game();

        // Update OpenGL
        gl_render();

        // Swap Buffers
        platform_swap_buffers();
    }
    FP_LOG("======= Game Loop End =======");

    // Return 0 when game loop is done (exitting appliation)
    FP_LOG("Shutdown complete");
    return 0;
}