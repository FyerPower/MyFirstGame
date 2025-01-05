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

bool just_pressed(GameInputType type)
{
    KeyMapping mapping = gameState->keyMappings[type];
    for (int idx = 0; idx < mapping.keys.count; idx++) {
        if (input->keys[mapping.keys[idx]].justPressed) {
            return true;
        }
    }
    return false;
}

bool is_down(GameInputType type)
{
    KeyMapping mapping = gameState->keyMappings[type];
    for (int idx = 0; idx < mapping.keys.count; idx++) {
        if (input->keys[mapping.keys[idx]].isDown) {
            FP_LOG("Mapping Size: %d", mapping.keys.count);
            return true;
        }
    }
    return false;
}

void checkForHotReloadUpdates(GameState* gameStateIn, RenderData* renderDataIn, Input* inputIn)
{
    // Because we hot-reload the game, we need to ensure we're using the correct game state
    if (renderData != renderDataIn) {
        gameState = gameStateIn;
        renderData = renderDataIn;
        input = inputIn;
    }
}

void initializeKeybinds()
{
    gameState->keyMappings[MOVE_UP].keys.add(KEY_W);
    gameState->keyMappings[MOVE_UP].keys.add(KEY_UP);
    gameState->keyMappings[MOVE_LEFT].keys.add(KEY_A);
    gameState->keyMappings[MOVE_LEFT].keys.add(KEY_LEFT);
    gameState->keyMappings[MOVE_DOWN].keys.add(KEY_S);
    gameState->keyMappings[MOVE_DOWN].keys.add(KEY_DOWN);
    gameState->keyMappings[MOVE_RIGHT].keys.add(KEY_D);
    gameState->keyMappings[MOVE_RIGHT].keys.add(KEY_RIGHT);
}

void handleInputActions()
{
    if (just_pressed(MOVE_LEFT)) {
        FP_LOG("Keybind Event Triggered: Move Left");
        gameState->playerPos.x -= 1;
    }
    if (just_pressed(MOVE_RIGHT)) {
        FP_LOG("Keybind Event Triggered: Move Right");
        gameState->playerPos.x += 1;
    }
    if (just_pressed(MOVE_UP)) {
        FP_LOG("Keybind Event Triggered: Move Up");
        gameState->playerPos.y -= 1;
    }
    if (just_pressed(MOVE_DOWN)) {
        FP_LOG("Keybind Event Triggered: Move Down");
        gameState->playerPos.y += 1;
    }
}

// #############################################################################
//                           Game Functions(exposed)
// #############################################################################

EXPORT_FN void update_game(GameState* gameStateIn, RenderData* renderDataIn, Input* inputIn)
{
    checkForHotReloadUpdates(gameStateIn, renderDataIn, inputIn);

    // Perform initialization logic
    if (!gameState->initialized) {
        initializeKeybinds();
        renderData->gameCamera.dimensions = {WORLD_WIDTH, WORLD_HEIGHT};
        gameState->initialized = true;
        FP_LOG("Game Initialized");
    }

    // Update gameCamera location
    renderData->gameCamera.position.x = 0;
    renderData->gameCamera.position.y = 0;

    // Draw some sprites
    draw_sprite(SPRITE_PLAYER, Vec2{32.0f, 32.0f});
    draw_sprite(SPRITE_PLAYER, Vec2{-32.0f, 32.0f});
    draw_sprite(SPRITE_PLAYER, Vec2{32.0f, -32.0f});
    draw_sprite(SPRITE_PLAYER, Vec2{-32.0f, -32.0f});

    draw_sprite(SPRITE_PLAYER, gameState->playerPos);

    handleInputActions();
}