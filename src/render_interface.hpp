#pragma once

// ###############################################
// #tag Imcludes
// ###############################################

#include "libs/libs.hpp"
#include "shaders/shared_header.hpp"
#include "game/camera.hpp"
#include "game/sprite.hpp"
#include "game/world.hpp"

// #############################################################################
// #tag Constants
// #############################################################################

// #############################################################################
// #tag Structs
// #############################################################################

// A store for all of our Rendered Objects
class RenderData
{
  public:
    OrthographicCamera2D gameCamera;
    OrthographicCamera2D uiCamera;

    Array<Transform, 1000> transforms; // Transformation Store
    // Array<Material, 1000> materials;   // Transformation Store
};

// #############################################################################
// #tag Globals
// #############################################################################

static RenderData* renderData;

// #############################################################################
// #tag Utilities
// #############################################################################

/**
 * Convert a screen position to a world position using the game camera.
 * Given the top left corner of the screen is (0, 0) and the bottom right is (screenSize.x, screenSize.y)
 * Given the top left corner of the camera is (camera.position.x - camera.dimensions.x / 2, camera.position.y + camera.dimensions.y / 2)
 * Given the bottom right corner of the camera is (camera.position.x + camera.dimensions.x / 2, camera.position.y - camera.dimensions.y / 2)
 */
IVec2 screen_to_world(IVec2 screenPos)
{
    // Get the zoomed dimensions of the camera
    Vec2 zoomedDimensions = renderData->gameCamera.getZoomedDimensions();

    // Calculate the scale of the camera
    Vec2 scale = zoomedDimensions / vec_2(input->screenSize);

    // Calculate the offset of the camera
    Vec2 offset = renderData->gameCamera.position;

    // Calculate the world position
    IVec2 worldPos = {(int)(screenPos.x * scale.x + offset.x), (int)(screenPos.y * scale.y - offset.y)};

    // Return the world position relative to the top left corner of the screen
    return {worldPos.x - ((int)zoomedDimensions.x / 2), worldPos.y - ((int)zoomedDimensions.y / 2)};
};

// #############################################################################
// #tag Functions
// #############################################################################

void draw_tile(Tile* tile)
{
    // Build the Transform
    Transform transform = {};

    // Position to the center of the sprite
    transform.pos = vec_2(tile->getPos()) - vec_2(tile->sprite->size) / 2.0f;
    transform.size = vec_2(tile->sprite->size);
    transform.spriteOffset = tile->sprite->offset;
    transform.spriteSize = tile->sprite->size;

    // Add the Transform to the RenderData
    renderData->transforms.add(transform);
}

// void draw_sprite(Sprite* sprite, Vec2 pos)
// {
//     // Add the transform to the RenderData
//     renderData->transforms.add({
//         // Position and Size
//         .pos = pos - vec_2(sprite->size) / 2.0f,
//         .size = vec_2(sprite->size),

//         // Define the type of transform
//         .type = TRANSFORM_TYPE_SPRITE,
//         .spriteOffset = sprite->offset,
//         .spriteSize = sprite->size,
//     });
// };

void draw_sprite(Sprite* sprite, Vec2 pos)
{
    // Build the Transform
    Transform transform = {};
    transform.pos = pos - vec_2(sprite->size) / 2.0f;
    transform.size = vec_2(sprite->size);
    transform.spriteOffset = sprite->offset;
    transform.spriteSize = sprite->size;

    //
    renderData->transforms.add(transform);
};

void draw_sprite(Sprite* sprite, IVec2 pos)
{
    draw_sprite(sprite, vec_2(pos));
};

void draw_rect(IRect rect, const Color& color) {
    // Transform transform = {};
    // transform.pos = vec_2(rect.pos);
    // transform.size = vec_2(rect.size);
    // transform.renderOptions = RENDERING_OPTION_OUTLINE;
    // renderData->transforms.add(transform);
};