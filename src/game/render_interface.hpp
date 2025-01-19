#pragma once

// ###############################################
// #tag Imcludes
// ###############################################

#include "shared/libs/libs.hpp"
#include "shared/models/render_data.hpp"
#include "shared/camera.hpp"
#include "game/models/sprite.hpp"
#include "game/models/world.hpp"

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

int get_material_idx(Material material = {})
{
    // Check if the material already exists
    for (int materialIdx = 0; materialIdx < renderData->materials.count; materialIdx++) {
        if (renderData->materials[materialIdx] == material) {
            return materialIdx;
        }
    }

    // Add the material to the list and return its index
    return renderData->materials.add(material);
}

// #############################################################################
// #tag Functions
// #############################################################################

void draw_tile(Tile* tile)
{
    // Get the Sprite
    Sprite* sprite = tile->sprite;
    renderData->transforms.add({
        // Position and Size
        .pos = vec_2(tile->getWorldPos()) - vec_2(sprite->size) / 2.0f,
        .size = vec_2(sprite->size),
        // Details about the type of transformation
        .renderOptions = RENDERING_OPTION_NONE,
        .transformType = TRANSFORM_TYPE_SPRITE,
        // Sprite Details
        .spriteOffset = sprite->offset,
        .spriteSize = sprite->size,
    });
}

void draw_sprite(Sprite* sprite, Vec2 pos, int renderOptions)
{
    renderData->transforms.add({
        // Position and Size
        .pos = pos - vec_2(sprite->size) / 2.0f,
        .size = vec_2(sprite->size),
        // Details about the type of transformation
        .renderOptions = renderOptions,
        .transformType = TRANSFORM_TYPE_SPRITE,
        // Sprite Details
        .spriteOffset = sprite->offset,
        .spriteSize = sprite->size,
    });
};

void draw_sprite(Sprite* sprite, Vec2 pos)
{
    draw_sprite(sprite, pos, RENDERING_OPTION_NONE);
};

void draw_sprite(Sprite* sprite, IVec2 pos, int renderOptions)
{
    draw_sprite(sprite, vec_2(pos), renderOptions);
};
void draw_sprite(Sprite* sprite, IVec2 pos)
{
    draw_sprite(sprite, vec_2(pos));
};

void draw_outline(IRect rect, const Color& color)
{
    renderData->transforms.add({
        // Position and Size
        .pos = vec_2(rect.pos),
        .size = vec_2(rect.size),
        // Details about the type of transformation
        .renderOptions = RENDERING_OPTION_NONE,
        .transformType = TRANSFORM_TYPE_OUTLINE,
        // Material Details
        .materialIndex = get_material_idx({.color = color}),
    });
};

void draw_fill(IRect rect, const Color& color)
{
    renderData->transforms.add({
        // Position and Size
        .pos = vec_2(rect.pos),
        .size = vec_2(rect.size),
        // Details about the type of transformation
        .renderOptions = RENDERING_OPTION_NONE,
        .transformType = TRANSFORM_TYPE_FILL,
        // Material Details
        .materialIndex = get_material_idx({.color = color}),
    });
};