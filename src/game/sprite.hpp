#pragma once

// ###############################################
// #tag Imcludes
// ###############################################

#include <array>
#include "../libs/libs.hpp"

// ###############################################
// #tag Constants
// ###############################################

// ###############################################
// #tag Structs
// ###############################################

class Sprite
{
  public:
    IVec2 offset;
    IVec2 size;
    IRect hitbox;
    bool isVisible = true;

    Sprite(IVec2 offset, IVec2 size) : offset(offset), size(size)
    {}

    Sprite(IVec2 offset, IVec2 size, IRect hitbox) : offset(offset), size(size), hitbox(hitbox)
    {}

    ~Sprite()
    {}
};

enum SpriteID {
    // Row 1
    SPRITE_GRASS_GRASS_HILL_1,
    SPRITE_GRASS_GRASS_HILL_2,
    SPRITE_GRASS_GRASS_HILL_3,
    SPRITE_GRASS_LUSH_HILL_1,
    SPRITE_GRASS_LUSH_HILL_2,
    SPRITE_GRASS_LUSH_HILL_3,
    // Row 2
    SPRITE_GRASS_GRASS_HILL_4,
    SPRITE_GRASS,
    SPRITE_GRASS_GRASS_HILL_5,
    SPRITE_GRASS_LUSH_HILL_4,
    SPRITE_GRASS_LUSH,
    SPRITE_GRASS_LUSH_HILL_5,
    // Row 3
    SPRITE_GRASS_GRASS_HILL_6,
    SPRITE_GRASS_GRASS_HILL_7,
    SPRITE_GRASS_GRASS_HILL_8,
    SPRITE_GRASS_LUSH_HILL_6,
    SPRITE_GRASS_LUSH_HILL_7,
    SPRITE_GRASS_LUSH_HILL_8,
    // Row 4
    SPRITE_GRASS_GRASS_HILL_9,
    SPRITE_GRASS_GRASS_HILL_10,
    SPRITE_GRASS_VAR1,
    SPRITE_GRASS_LUSH_HILL_9,
    SPRITE_GRASS_LUSH_HILL_10,
    SPRITE_GRASS_LUSH_VAR1,
    // Row 5
    SPRITE_GRASS_GRASS_HILL_11,
    SPRITE_GRASS_GRASS_HILL_12,
    SPRITE_GRASS_VAR2,
    SPRITE_GRASS_LUSH_HILL_11,
    SPRITE_GRASS_LUSH_HILL_12,
    SPRITE_GRASS_LUSH_VAR2,
    // Row 6
    SPRITE_PLAYER,
    SPRITE_PLAYER_RIGHT,
    SPRITE_PLAYER_BACK,
    SPRITE_COUNT
};

// ###############################################
// #tag Functions
// ###############################################

std::array<Sprite*, SPRITE_COUNT> initializeSprites()
{
    std::array<Sprite*, SPRITE_COUNT> spriteArray;
    // Row 1
    spriteArray[SPRITE_GRASS_GRASS_HILL_1] = new Sprite({0, 0}, {16, 16}, {0, 0, 16, 16});
    spriteArray[SPRITE_GRASS_GRASS_HILL_2] = new Sprite({16, 0}, {16, 16}, {0, 0, 16, 16});
    spriteArray[SPRITE_GRASS_GRASS_HILL_3] = new Sprite({32, 0}, {16, 16}, {0, 0, 16, 16});
    spriteArray[SPRITE_GRASS_LUSH_HILL_1] = new Sprite({48, 0}, {16, 16}, {0, 0, 16, 16});
    spriteArray[SPRITE_GRASS_LUSH_HILL_2] = new Sprite({64, 0}, {16, 16}, {0, 0, 16, 16});
    spriteArray[SPRITE_GRASS_LUSH_HILL_3] = new Sprite({80, 0}, {16, 16}, {0, 0, 16, 16});
    // Row 2
    spriteArray[SPRITE_GRASS_GRASS_HILL_4] = new Sprite({0, 16}, {16, 16}, {0, 0, 16, 16});
    spriteArray[SPRITE_GRASS] = new Sprite({16, 16}, {16, 16});
    spriteArray[SPRITE_GRASS_GRASS_HILL_5] = new Sprite({32, 16}, {16, 16}, {0, 0, 16, 16});
    spriteArray[SPRITE_GRASS_LUSH_HILL_4] = new Sprite({48, 16}, {16, 16}, {0, 0, 16, 16});
    spriteArray[SPRITE_GRASS_LUSH] = new Sprite({64, 16}, {16, 16});
    spriteArray[SPRITE_GRASS_LUSH_HILL_5] = new Sprite({80, 16}, {16, 16}, {0, 0, 16, 16});
    // Row 3
    spriteArray[SPRITE_GRASS_GRASS_HILL_6] = new Sprite({0, 32}, {16, 16}, {0, 0, 16, 16});
    spriteArray[SPRITE_GRASS_GRASS_HILL_7] = new Sprite({16, 32}, {16, 16}, {0, 0, 16, 16});
    spriteArray[SPRITE_GRASS_GRASS_HILL_8] = new Sprite({32, 32}, {16, 16}, {0, 0, 16, 16});
    spriteArray[SPRITE_GRASS_LUSH_HILL_6] = new Sprite({48, 32}, {16, 16}, {0, 0, 16, 16});
    spriteArray[SPRITE_GRASS_LUSH_HILL_7] = new Sprite({64, 32}, {16, 16}, {0, 0, 16, 16});
    spriteArray[SPRITE_GRASS_LUSH_HILL_8] = new Sprite({80, 32}, {16, 16}, {0, 0, 16, 16});
    // Row 4
    spriteArray[SPRITE_GRASS_GRASS_HILL_9] = new Sprite({0, 48}, {16, 16}, {0, 0, 16, 16});
    spriteArray[SPRITE_GRASS_GRASS_HILL_10] = new Sprite({16, 48}, {16, 16}, {0, 0, 16, 16});
    spriteArray[SPRITE_GRASS_VAR1] = new Sprite({32, 48}, {16, 16});
    spriteArray[SPRITE_GRASS_LUSH_HILL_9] = new Sprite({48, 48}, {16, 16}, {0, 0, 16, 16});
    spriteArray[SPRITE_GRASS_LUSH_HILL_10] = new Sprite({64, 48}, {16, 16}, {0, 0, 16, 16});
    spriteArray[SPRITE_GRASS_LUSH_VAR1] = new Sprite({80, 48}, {16, 16});
    // Row 5
    spriteArray[SPRITE_GRASS_GRASS_HILL_11] = new Sprite({0, 64}, {16, 16}, {0, 0, 16, 16});
    spriteArray[SPRITE_GRASS_GRASS_HILL_12] = new Sprite({16, 64}, {16, 16}, {0, 0, 16, 16});
    spriteArray[SPRITE_GRASS_VAR2] = new Sprite({32, 64}, {16, 16});
    spriteArray[SPRITE_GRASS_LUSH_HILL_11] = new Sprite({48, 64}, {16, 16}, {0, 0, 16, 16});
    spriteArray[SPRITE_GRASS_LUSH_HILL_12] = new Sprite({64, 64}, {16, 16}, {0, 0, 16, 16});
    spriteArray[SPRITE_GRASS_LUSH_VAR2] = new Sprite({80, 64}, {16, 16});
    // Row 6
    spriteArray[SPRITE_PLAYER] = new Sprite({0, 80}, {32, 32}, {8, 16, 16, 16});
    spriteArray[SPRITE_PLAYER_RIGHT] = new Sprite({32, 80}, {32, 32}, {8, 16, 16, 16});
    spriteArray[SPRITE_PLAYER_BACK] = new Sprite({64, 80}, {32, 32}, {8, 16, 16, 16});

    FP_LOG("Initialized Sprites");

    return spriteArray;
}