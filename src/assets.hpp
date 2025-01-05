#pragma once

// Include all of our libs
#include "libs/libs.hpp"

// #############################################################################
//                           Assets Constants
// #############################################################################

// #############################################################################
//                           Assets Structs
// #############################################################################

enum SpriteID {
    SPRITE_PLAYER,

    SPRITE_COUNT
};

struct Sprite {
    IVec2 atlasOffset;
    IVec2 spriteSize;
};

// #############################################################################
//                           Assets Functions
// #############################################################################

Sprite get_sprite(SpriteID spriteID)
{
    Sprite sprite = {};
    switch (spriteID) {
        case SPRITE_PLAYER: {
            sprite.atlasOffset = {0, 0};
            sprite.spriteSize = {32, 32};
            break;
        }
        default: {
            // TODO: Add Missing Texture
            sprite.atlasOffset = {0, 0};
            sprite.spriteSize = {0, 0};
        }
    }
    return sprite;
}