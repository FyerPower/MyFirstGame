#pragma once

// ###############################################
// #tag Includes
// ###############################################

#include "libs/libs.hpp"
#include "input.hpp"
#include "render_interface.hpp"

#include "game/config.hpp"
#include "game/player.hpp"
#include "game/world.hpp"

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

extern "C++" {
EXPORT_FN void update_game(GameState* gameStateIn, RenderData* renderDataIn, Input* inputIn, float deltaTime);
}