// ###############################################
// #tag Includes
// ###############################################

#include "shared/libs/libs.hpp"
#include "shared/models/input.hpp"
#include "game/game.hpp"

#define APIENTRY
#define GL_GLEXT_PROTOTYPES
#include "glcorearb.h"

#include <chrono> // Used to get Delta Time
#include <string> // for string class

// ###############################################
// #tag Platforms
// ###############################################

static KeyCodeID KeyCodeLookupTable[KEY_COUNT];
#include "shared/platform/platform.hpp"

#ifdef _WIN32
#include "shared/platform/platform_windows.cpp"
#elif __linux__
#include "shared/platform/platform_linux.cpp"
#elif __APPLE__
#include "shared/platform/platform_mac.cpp"
#endif

#include "open_gl_renderer.cpp"

// ###############################################
// #tag Functions
// ###############################################

double get_delta_time()
{
    // Only executed once when entering the function (static)
    static auto lastTime = std::chrono::steady_clock::now();
    auto currentTime = std::chrono::steady_clock::now();
    // seconds
    double delta = std::chrono::duration<double>(currentTime - lastTime).count();
    lastTime = currentTime;
    return delta;
}

// ###############################################
// #tag Hot-Reloading
// ###############################################

// This is the function pointer to update_game in game.cpp
typedef decltype(update_game) update_game_type;
static update_game_type* update_game_ptr;

// Wrapper Function
void update_game(GameState* gameStateIn, RenderData* renderDataIn, Input* inputIn, float deltaTime)
{
    if (update_game_ptr) {
        update_game_ptr(gameStateIn, renderDataIn, inputIn, deltaTime);
    } else {
        Logger::asssert(false, "update_game_ptr is null");
    }
}

void reload_game_dll(BumpAllocator* transientStorage)
{
    static void* gameDLL;
    static long long lastEditTimestampGameDLL;
    static const char* gameDLLPath = "build/builds/game_load.dll";
    static const char* gameDLLLoadedPath = "build/builds/game2.dll";

    long long currentTimestampGameDLL = get_timestamp(gameDLLPath);
    if (currentTimestampGameDLL > lastEditTimestampGameDLL) {
        if (gameDLL) {
            bool freeResult = platform_free_dynamic_library(gameDLL);
            Logger::asssert(freeResult, "Failed to free (%s)", gameDLLPath);
            gameDLL = nullptr;
            Logger::log("Freed (%s)", gameDLLPath);
        }

        Sleep(10);
        while (!copy_file(gameDLLPath, gameDLLLoadedPath, transientStorage)) {
            Logger::log("Failed to copy DLL Files, Retrying...");
        }
        Logger::log("Copied (%s) into (%s)", gameDLLPath, gameDLLLoadedPath);

        gameDLL = platform_load_dynamic_library(gameDLLLoadedPath);
        Logger::asssert(gameDLL, "Failed to load (%s)", gameDLLLoadedPath);

        update_game_ptr = (update_game_type*)platform_load_dynamic_function(gameDLL, "update_game");
        Logger::asssert(update_game_ptr, "Failed to load update_game function");

        Logger::log("Successfully loaded (%s)", gameDLLLoadedPath);
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

    // Allocate the GameState into Memory
    void* gameState_ptr = bump_alloc(&persistentStorage, sizeof(GameState));
    if (!gameState_ptr) {
        Logger::error("Failed to allow GameState");
        return -1;
    }
    gameState = new (gameState_ptr) GameState();

    // Allocate the RenderData into Memory
    void* renderData_ptr = bump_alloc(&persistentStorage, sizeof(RenderData));
    if (!renderData_ptr) {
        Logger::error("Failed to allow RenderData");
        return -1;
    }
    renderData = new (renderData_ptr) RenderData();

    // Allocate the Input into Memory
    input = (Input*)bump_alloc(&persistentStorage, sizeof(Input));
    if (!input) {
        Logger::error("Failed to allow Input");
        return -1;
    }

    // Create the Window
    input->screenSize.x = WINDOW_SIZE_X;
    input->screenSize.y = WINDOW_SIZE_Y;
    std::string windowTitle = DEBUG_MODE == true ? (std::string(WINDOW_TITLE) + " (Debug Mode)") : WINDOW_TITLE;
    platform_create_window(input->screenSize.x, input->screenSize.y, windowTitle.c_str());
    Logger::log("Window Created");

    // Assign keys per platform
    platform_fill_keycode_lookup_table();

    // Initialize OpenGL
    gl_init(&transientStorage);
    Logger::log("Initialized OpenGL");

    // Main Game Loop
    Logger::log("======= Game Loop Begin =======");
    while (isRunning) {
        // Get Delta Time
        float deltaTime = (float)get_delta_time();

        // Check to see if the Game.DLL needs updating and update accordingly
        reload_game_dll(&transientStorage);

        // Update the Window
        platform_update_window();

        // Update Game
        update_game(gameState, renderData, input, deltaTime);

        // Update OpenGL
        gl_render(&transientStorage);

        // Swap Buffers
        platform_swap_buffers();

        // Reset Storage (as its transient)
        transientStorage.used = 0;
    }
    Logger::log("======= Game Loop End =======");

    // Return 0 when game loop is done (exitting appliation)
    Logger::log("Shutdown complete");
    return 0;
}
