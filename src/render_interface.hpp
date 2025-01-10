#pragma once

// ###############################################
// #tag Imcludes
// ###############################################

#include "game/camera.hpp"
#include "assets.hpp"

// #############################################################################
// #tag Constants
// #############################################################################

// #############################################################################
// #tag Structs
// #############################################################################

// Transformation Object that holds information about what is being rendered
struct Transform {
    Vec2 pos;          // The position in the world
    Vec2 size;         // The size the sprite should be rendered as
    IVec2 atlasOffset; // The offset (in pixels) in the texture file this a sprite is located
    IVec2 spriteSize;  // The size (in pixels) this sprite is
};

// A store for all of our Rendered Objects
class RenderData
{
  public:
    OrthographicCamera2D gameCamera;
    OrthographicCamera2D uiCamera;

    Array<Transform, 1000> transforms; // Transformation Store
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
    renderData->transforms.add(transform);
};

void draw_sprite(SpriteID spriteID, IVec2 pos)
{
    draw_sprite(spriteID, vec_2(pos));
};

void draw_quad(Transform transform)
{
    renderData->transforms.add(transform);
};

void draw_quad(Vec2 pos, Vec2 size)
{
    Transform transform = {};
    transform.pos = pos - size / 2.0f;
    transform.size = size;
    transform.atlasOffset = {0, 0};
    transform.spriteSize = {1, 1}; // Indexing into white
    renderData->transforms.add(transform);
};