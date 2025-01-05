#include "libs/libs.hpp"
#include "game.hpp"
#include "assets.hpp"
#include "render_interface.hpp"

// #############################################################################
//                           Game Constants
// #############################################################################

// #############################################################################
//                           Game Structs
// #############################################################################

// #############################################################################
//                           Game Functions
// #############################################################################

EXPORT_FN void update_game(RenderData* renderDataIn, Input* inputIn)
{
    if (renderData != renderDataIn) {
        renderData = renderDataIn;
        input = inputIn;
    }

    // draw_sprite(SPRITE_PLAYER, {100.0f, 100.0f}, {100.0f, 100.0f});
    // draw_sprite(SPRITE_PLAYER, {-100.0f, 100.0f}, {100.0f, 100.0f});
    // draw_sprite(SPRITE_PLAYER, {100.0f, -100.0f}, {100.0f, 100.0f});
    // draw_sprite(SPRITE_PLAYER, {-100.0f, -100.0f}, {100.0f, 100.0f});

    for (int x = 0; x < 2; x++) {
        for (int y = 0; y < 2; y++) {
            draw_sprite(SPRITE_PLAYER, {x * 100.0f, y * 100.0f}, {100.0f, 100.0f});
        }
    }
}