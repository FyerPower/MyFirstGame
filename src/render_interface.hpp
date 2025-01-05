#pragma once

#include "assets.hpp"

// #############################################################################
//                           Renderer Constants
// #############################################################################

// Maximum number of sprites we can draw to the screen (at the moment)
constexpr int MAX_TRANSFORMS = 1000;

// #############################################################################
//                           Renderer Structs
// #############################################################################

// Transformation Object that holds information about what is being rendered
struct Transform {
    Vec2 pos;          // The position in the world
    Vec2 size;         // The size the sprite should be rendered as
    IVec2 atlasOffset; // The offset (in pixels) in the texture file this a sprite is located
    IVec2 spriteSize;  // The size (in pixels) this sprite is
};

// A store for all of our Rendered Objects
struct RenderData {
    OrthographicCamera2D gameCamera;
    OrthographicCamera2D uiCamera;

    int transformCount;                   // Number of transformations currently done
    Transform transforms[MAX_TRANSFORMS]; // Transformation Store
};

// #############################################################################
//                           Renderer Globals
// #############################################################################

static RenderData* renderData;

// #############################################################################
//                           Renderer Utilities
// #############################################################################

IVec2 screen_to_world(IVec2 screenPos)
{
    OrthographicCamera2D camera = renderData->gameCamera;

    int xPos = screenPos.x / input->screenSize.x * (int)camera.dimensions.x; // [0; dimensions.x]
    // Offset using dimensions and position
    xPos += -camera.dimensions.x / 2.0f + camera.position.x;

    int yPos = screenPos.y / input->screenSize.y * (int)camera.dimensions.y; // [0; dimensions.y]
    // Offset using dimensions and position
    yPos += -camera.dimensions.y / 2.0f - camera.position.y;
    return {xPos, yPos};
};

// #############################################################################
//                           Renderer Functions
// #############################################################################

void draw_sprite(SpriteID spriteID, Vec2 pos)
{
    // Get the Sprite
    Sprite sprite = get_sprite(spriteID);

    // Build the Transform
    Transform transform = {};

    // Position to the center of the sprite
    transform.pos = pos - vec_2(sprite.spriteSize) / 2.0f;

    //
    transform.size = vec_2(sprite.spriteSize);
    transform.atlasOffset = sprite.atlasOffset;
    transform.spriteSize = sprite.spriteSize;

    //
    renderData->transforms[renderData->transformCount++] = transform;
};

void draw_sprite(SpriteID spriteID, IVec2 pos)
{
    draw_sprite(spriteID, vec_2(pos));
};