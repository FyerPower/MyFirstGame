#pragma once

#include "shared/libs/libs.hpp"

// ###############################################
// #tag Constants
// ###############################################

// The size of the world in tiles
constexpr int TILESIZE = 16;
constexpr IVec2 WORLD_GRID = {30, 20};
constexpr int WORLD_WIDTH = WORLD_GRID.x * TILESIZE;
constexpr int WORLD_HEIGHT = WORLD_GRID.y * TILESIZE;

// Update Loop
constexpr int UPDATES_PER_SECOND = 60;
constexpr double UPDATE_DELAY = 1.0 / UPDATES_PER_SECOND;