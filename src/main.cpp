// ###############################################
// #tag Includes
// ###############################################

#include "libs/libs.hpp"
#include "input.hpp"
#include "game.hpp"

#define APIENTRY
#define GL_GLEXT_PROTOTYPES
#include "glcorearb.h"

// ###############################################
// #tag Platforms
// ###############################################

static KeyCodeID KeyCodeLookupTable[KEY_COUNT];
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
// #tag Functions
// ###############################################

// ###############################################
// #tag Hot-Reloading
// ###############################################

// This is the function pointer to update_game in game.cpp
typedef decltype(update_game) update_game_type;
static update_game_type* update_game_ptr;

// Wrapper Function
void update_game(GameState* gameStateIn, RenderData* renderDataIn, Input* inputIn)
{
    update_game_ptr(gameStateIn, renderDataIn, inputIn);
}

void reload_game_dll(BumpAllocator* transientStorage)
{
    static void* gameDLL;
    static long long lastEditTimestampGameDLL;

    long long currentTimestampGameDLL = get_timestamp("builds/game.dll");
    if (currentTimestampGameDLL > lastEditTimestampGameDLL) {
        if (gameDLL) {
            bool freeResult = platform_free_dynamic_library(gameDLL);
            FP_ASSERT(freeResult, "Failed to free game.dll");
            gameDLL = nullptr;
            FP_LOG("Freed game.dll");
        }

        while (!copy_file("builds/game.dll", "builds/game_load.dll", transientStorage)) {
            Sleep(10);
        }
        FP_LOG("Copied game.dll into game_load.dll");

        gameDLL = platform_load_dynamic_library("builds/game_load.dll");
        FP_ASSERT(gameDLL, "Failed to load game.dll");

        update_game_ptr = (update_game_type*)platform_load_dynamic_function(gameDLL, "update_game");
        FP_ASSERT(update_game_ptr, "Failed to load update_game function");
        lastEditTimestampGameDLL = currentTimestampGameDLL;
    }
}

// ###############################################
// #tag Main-Logic
// ###############################################

int main()
{
    // Create 50MB of memory that is transient (resets each game loop) and persistent.
    BumpAllocator transientStorage = make_bump_allocator(MB(50));
    BumpAllocator persistentStorage = make_bump_allocator(MB(50));

    input = (Input*)bump_alloc(&persistentStorage, sizeof(Input));
    if (!input) {
        FP_ERROR("Failed to allow Input");
        return -1;
    }

    renderData = (RenderData*)bump_alloc(&persistentStorage, sizeof(RenderData));
    if (!renderData) {
        FP_ERROR("Failed to allow RenderData");
        return -1;
    }

    gameState = (GameState*)bump_alloc(&persistentStorage, sizeof(GameState));
    if (!gameState) {
        FP_ERROR("Failed to allow GameState");
        return -1;
    }

    // Create the Window
    input->screenSize.x = 1280;
    input->screenSize.y = 720;
    platform_create_window(input->screenSize.x, input->screenSize.y, "My Game");
    FP_LOG("Window Created");

    // Assign keys per platform
    platform_fill_keycode_lookup_table();

    // Initialize OpenGL
    gl_init(&transientStorage);
    FP_LOG("Initialized OpenGL");

    // Main Game Loop
    FP_LOG("======= Game Loop Begin =======");
    while (isRunning) {
        reload_game_dll(&transientStorage);

        // Update the Window
        platform_update_window();

        // Update Game
        update_game(gameState, renderData, input);

        // Update OpenGL
        gl_render(&transientStorage);

        // Swap Buffers
        platform_swap_buffers();

        // Reset Storage (as its transient)
        transientStorage.used = 0;
    }
    FP_LOG("======= Game Loop End =======");

    // Return 0 when game loop is done (exitting appliation)
    FP_LOG("Shutdown complete");
    return 0;
}
