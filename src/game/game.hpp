#pragma once

// ###############################################
// #tag Includes
// ###############################################

#include <array>
#include "shared/libs/libs.hpp"
#include "shared/models/input.hpp"
#include "game/render_interface.hpp"

#include "game/models/player.hpp"
#include "game/models/world.hpp"
#include "game/models/sprite.hpp"

// ###############################################
// #tag Structs
// ###############################################

enum GameInputType {
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
    JUMP,
    MOUSE_LEFT,
    MOUSE_RIGHT,
    MOUSE_SCROLL_UP,
    MOUSE_SCROLL_DOWN,
    GAME_INPUT_COUNT
};

struct KeyMapping {
    Array<KeyCodeID, 3> keys;
};

struct GameState {
  public:
    bool initialized = false;
    float internalTimer;
    int tickCounter = 0;

    Player* player;
    World* world;

    //
    std::array<Sprite*, SPRITE_COUNT> spriteArray;
    Array<IVec2, 21> tileCoords;

    // Key Mapping
    KeyMapping keyMappings[GAME_INPUT_COUNT];

    GameState()
    {
        world = new World();
        player = new Player();
    }
};

// ###############################################
// #tag Globals
// ###############################################

static GameState* gameState;

// ###############################################
// #tag Functions (Exposed)
// ###############################################

#ifdef _WIN32
#define EXPORT_FN __declspec(dllexport)
#elif __linux__
#define EXPORT_FN
#elif __APPLE__
#define EXPORT_FN
#endif

// TODO: Why is this "C" and not "C++"?
extern "C" {
EXPORT_FN void update_game(GameState* gameStateIn, RenderData* renderDataIn, Input* inputIn, float deltaTime);
}