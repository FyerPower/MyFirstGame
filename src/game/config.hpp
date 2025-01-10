#pragma once

#include "../libs/libs.hpp"

// ###############################################
// #tag Constants
// ###############################################

constexpr int WORLD_WIDTH = 320;
constexpr int WORLD_HEIGHT = 160;
constexpr int TILESIZE = 16;

// The size of the world in tiles
constexpr IVec2 WORLD_GRID = {WORLD_WIDTH / TILESIZE, WORLD_HEIGHT / TILESIZE};

// The size of the screen in tiles

constexpr int UPDATES_PER_SECOND = 60;
constexpr double UPDATE_DELAY = 1.0 / UPDATES_PER_SECOND;