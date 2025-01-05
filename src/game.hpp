#pragma once

#include "libs/libs.hpp"
#include "input.hpp"
#include "render_interface.hpp"

// #############################################################################
//                           Game Constants
// #############################################################################

constexpr int tset = 5;

// #############################################################################
//                           Game Structs
// #############################################################################

struct GameState {
    bool initialized = false;
    IVec2 playerPos;
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