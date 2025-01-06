#pragma once

#include "libs/libs.hpp"
#include "input.hpp"
#include "render_interface.hpp"

// #############################################################################
//                           Game Constants
// #############################################################################

constexpr int WORLD_WIDTH = 320;
constexpr int WORLD_HEIGHT = 180;
constexpr int TILESIZE = 16;
constexpr IVec2 WORLD_GRID = {WORLD_WIDTH / TILESIZE, WORLD_HEIGHT / TILESIZE};

// #############################################################################
//                           Game Structs
// #############################################################################

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

struct GameState {
    bool initialized = false;
    IVec2 playerPos;

    Array<IVec2, 21> tileCoords;
    Tile worldGrid[WORLD_GRID.x][WORLD_GRID.y];
    KeyMapping keyMappings[GAME_INPUT_COUNT];
};

// #############################################################################
//                           Game Globals
// #############################################################################

static GameState* gameState;

// #############################################################################
//                           Game Functions (Exposed)
// #############################################################################

extern "C" {
EXPORT_FN void update_game(GameState* gameStateIn, RenderData* renderDataIn, Input* inputIn);
}