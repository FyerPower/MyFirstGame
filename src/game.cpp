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

Tile* get_tile(int x, int y)
{
    Tile* tile = nullptr;
    if (x >= 0 && x < WORLD_GRID.x && y >= 0 && y < WORLD_GRID.y) {
        tile = &gameState->worldGrid[x][y];
    }
    return tile;
}

Tile* get_tile(IVec2 worldPos)
{
    int x = worldPos.x / TILESIZE;
    int y = worldPos.y / TILESIZE;
    return get_tile(x, y);
}

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
    gameState->keyMappings[MOUSE_LEFT].keys.add(KEY_MOUSE_LEFT);
    gameState->keyMappings[MOUSE_RIGHT].keys.add(KEY_MOUSE_RIGHT);

    // initialize gameCamera location
    renderData->gameCamera.position.x = 160;
    renderData->gameCamera.position.y = -90;
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

    if (is_down(MOUSE_LEFT)) {
        IVec2 worldPos = screen_to_world(input->mousePos);
        IVec2 mousePosWorld = input->mousePosWorld;
        Tile* tile = get_tile(worldPos);
        if (tile) {
            tile->isVisible = true;
        }
    }
    if (is_down(MOUSE_RIGHT)) {
        IVec2 worldPos = screen_to_world(input->mousePos);
        IVec2 mousePosWorld = input->mousePosWorld;
        Tile* tile = get_tile(worldPos);
        if (tile) {
            tile->isVisible = false;
        }
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
        // Tileset
        {
            IVec2 tilesPosition = {240, 0};
            for (int y = 0; y < 5; y++) {
                for (int x = 0; x < 4; x++) {
                    gameState->tileCoords.add({tilesPosition.x + x * TILESIZE, tilesPosition.y + y * TILESIZE});
                }
            }
            // Black inside
            gameState->tileCoords.add({tilesPosition.x, tilesPosition.y + 5 * TILESIZE});
        }
        renderData->gameCamera.dimensions = {WORLD_WIDTH, WORLD_HEIGHT};
        gameState->initialized = true;
        FP_LOG("Game Initialized");
    }

    // Drawing Tileset
    {
        // Neighbouring Tiles        Top    Left      Right       Bottom
        int neighbourOffsets[24] = {0, -1, -1, 0, 1, 0, 0, 1,
                                    // Topleft Topright Bottomleft Bottomright
                                    -1, -1, 1, -1, -1, 1, 1, 1,
                                    // Top2   Left2     Right2      Bottom2
                                    0, -2, -2, 0, 2, 0, 0, 2};
        // Topleft     = BIT(4) = 16
        // Toplright   = BIT(5) = 32
        // Bottomleft  = BIT(6) = 64
        // Bottomright = BIT(7) = 128
        for (int y = 0; y < WORLD_GRID.y; y++) {
            for (int x = 0; x < WORLD_GRID.x; x++) {
                // Draw Background
                Transform bgTransformation = {};
                bgTransformation.pos = {x * (float)TILESIZE, y * (float)TILESIZE};
                bgTransformation.size = {16, 16};
                bgTransformation.spriteSize = {16, 16};
                bgTransformation.atlasOffset = {192, 0};
                draw_quad(bgTransformation);

                Tile* tile = get_tile(x, y);
                if (!tile->isVisible) {
                    continue;
                }
                tile->neighbourMask = 0;
                int neighbourCount = 0;
                int extendedNeighbourCount = 0;
                int emptyNeighbourSlot = 0;
                // Look at the sorrounding 12 Neighbours
                for (int n = 0; n < 12; n++) {
                    Tile* neighbour = get_tile(x + neighbourOffsets[n * 2], y + neighbourOffsets[n * 2 + 1]);
                    // No neighbour means the edge of the world
                    if (!neighbour || neighbour->isVisible) {
                        tile->neighbourMask |= BIT(n);
                        if (n < 8) // Counting direct neighbours
                        {
                            neighbourCount++;
                        } else // Counting neighbours 1 Tile away
                        {
                            extendedNeighbourCount++;
                        }
                    } else if (n < 8) {
                        emptyNeighbourSlot = n;
                    }
                }
                if (neighbourCount == 7 && emptyNeighbourSlot >= 4) // We have a corner
                {
                    tile->neighbourMask = 16 + (emptyNeighbourSlot - 4);
                } else if (neighbourCount == 8 && extendedNeighbourCount == 4) {
                    tile->neighbourMask = 20;
                } else {
                    tile->neighbourMask = tile->neighbourMask & 0b1111;
                }

                // Draw Tile
                Transform transform = {};
                // Draw the Tile around the center
                transform.pos = {x * (float)TILESIZE, y * (float)TILESIZE};
                transform.size = {16, 16};
                transform.spriteSize = {16, 16};
                transform.atlasOffset = gameState->tileCoords[tile->neighbourMask];
                draw_quad(transform);
            }
        }
    }

    // Draw some sprites
    draw_sprite(SPRITE_PLAYER, Vec2{32.0f, 32.0f});
    draw_sprite(SPRITE_PLAYER, Vec2{64.0f, 32.0f});
    draw_sprite(SPRITE_PLAYER, Vec2{32.0f, 64.0f});
    draw_sprite(SPRITE_PLAYER, Vec2{64.0f, 64.0f});

    draw_sprite(SPRITE_PLAYER, gameState->playerPos);

    handleInputActions();
}