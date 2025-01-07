#pragma once

// ###############################################
// #tag Includes
// ###############################################

#include "libs/libs.hpp"
#include "input.hpp"
#include "render_interface.hpp"

// ###############################################
// #tag Constants
// ###############################################

constexpr int WORLD_WIDTH = 320;
constexpr int WORLD_HEIGHT = 160;
constexpr int TILESIZE = 16;
constexpr IVec2 WORLD_GRID = {WORLD_WIDTH / TILESIZE, WORLD_HEIGHT / TILESIZE};

constexpr int UPDATES_PER_SECOND = 20;
constexpr double UPDATE_DELAY = 1.0 / UPDATES_PER_SECOND;

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
    GAME_INPUT_COUNT
};

struct KeyMapping {
    Array<KeyCodeID, 3> keys;
};

struct Tile {
    int neighbourMask;
    bool isVisible;
};

struct Player {
    IVec2 position;
    IVec2 previousPosition;
    int health;
    int maxHealth = 60;
    int stamina;
    int maxStamina = 100;
};

struct GameState {
    bool initialized = false;
    float internalTimer;
    int tickCounter = 0;

    Player player;

    //
    Array<IVec2, 21> tileCoords;

    // World Grid
    Tile worldGrid[WORLD_GRID.x][WORLD_GRID.y];

    // Key Mapping
    KeyMapping keyMappings[GAME_INPUT_COUNT];
};

// ###############################################
// #tag Globals
// ###############################################

static GameState* gameState;

// ###############################################
// #tag Functions (Exposed)
// ###############################################

extern "C" {
EXPORT_FN void update_game(GameState* gameStateIn, RenderData* renderDataIn, Input* inputIn, float deltaTime);
}