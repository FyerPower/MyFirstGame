#include "libs/libs.hpp"
#include "game.hpp"
#include "assets.hpp"
#include "render_interface.hpp"

// #############################################################################
//                           Game Constants
// #############################################################################
constexpr int WORLD_WIDTH = 320;
constexpr int WORLD_HEIGHT = 180;
constexpr int TILESIZE = 8;

// #############################################################################
//                           Game Structs
// #############################################################################

// #############################################################################
//                           Game Functions
// #############################################################################

EXPORT_FN void update_game(GameState* gameStateIn, RenderData* renderDataIn, Input* inputIn)
{
    // Because we hot-reload the game, we need to ensure we're using the correct game state
    if (renderData != renderDataIn) {
        gameState = gameStateIn;
        renderData = renderDataIn;
        input = inputIn;
    }

    // Perform initialization logic
    if (!gameState->initialized) {
        renderData->gameCamera.dimensions = {WORLD_WIDTH, WORLD_HEIGHT};
        gameState->initialized = true;
    }

    // Update gameCamera location
    renderData->gameCamera.position.x = 0;
    renderData->gameCamera.position.y = 0;

    // Draw some sprites
    draw_sprite(SPRITE_PLAYER, {32.0f, 32.0f});
    draw_sprite(SPRITE_PLAYER, {-32.0f, 32.0f});
    draw_sprite(SPRITE_PLAYER, {32.0f, -32.0f});
    draw_sprite(SPRITE_PLAYER, {-32.0f, -32.0f});
}