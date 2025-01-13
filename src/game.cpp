// ###############################################
// #tag Includes
// ###############################################

#include "libs/libs.hpp"
#include "game.hpp"
#include "assets.hpp"
#include "render_interface.hpp"

// ###############################################
// #tag Constants
// ###############################################

// ###############################################
// #tag Structs
// ###############################################

// ###############################################
// #tag Functions
// ###############################################

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
            return true;
        }
    }
    return false;
}
bool is_control_pressed()
{
    return input->keys[KEY_CONTROL].isDown;
}
bool is_alt_pressed()
{
    return input->keys[KEY_ALT].isDown;
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
    gameState->keyMappings[MOUSE_SCROLL_UP].keys.add(KEY_MOUSE_SCROLL_UP);
    gameState->keyMappings[MOUSE_SCROLL_DOWN].keys.add(KEY_MOUSE_SCROLL_DOWN);

    // initialize gameCamera location
    renderData->gameCamera.position.x = 160;
    renderData->gameCamera.position.y = -80;
}

// ###############################################
// #tag Functions Tick Logic
// ###############################################

void handleInputActions(float deltaTime)
{
    World* world = gameState->world;

    if (is_down(MOUSE_LEFT)) {
        IVec2 worldPos = screen_to_world(input->mousePos);
        IVec2 mousePosWorld = input->mousePosWorld;
        Tile* tile = world->get_tile(worldPos);
        if (tile) {
            tile->isVisible = true;
        }
    }
    if (is_down(MOUSE_RIGHT)) {
        IVec2 worldPos = screen_to_world(input->mousePos);
        IVec2 mousePosWorld = input->mousePosWorld;
        Tile* tile = world->get_tile(worldPos);
        if (tile) {
            tile->isVisible = false;
        }
    }
    if (just_pressed(MOUSE_SCROLL_DOWN)) {
        FP_LOG("Key Pressed: MOUSE_SCROLL_DOWN");
        if (is_control_pressed()) {
            renderData->gameCamera.zoomOut();
        }
    }
    if (just_pressed(MOUSE_SCROLL_UP)) {
        FP_LOG("Key Pressed: MOUSE_SCROLL_UP");
        if (is_control_pressed()) {
            renderData->gameCamera.zoomIn();
        }
    }
}

/**
 * Analyze player->speed and move accordingly.
 */
void updatePlayerPosition()
{
    World* world = gameState->world;
    Player* player = gameState->player;
    player->previousPosition = player->position;

    // Handle keybinds for movement
    if (is_down(MOVE_LEFT)) {
        player->setSpeedX(-1);
    } else if (is_down(MOVE_RIGHT)) {
        player->setSpeedX(1);
    } else {
        player->setSpeedX(0);
    }
    if (is_down(MOVE_UP)) {
        player->setSpeedY(-1);
    } else if (is_down(MOVE_DOWN)) {
        player->setSpeedY(1);
    } else {
        player->setSpeedY(0);
    }

    static Vec2 remainder = {};
    // Move the Player in X until collision or moveX is exausted
    remainder.x += player->speed.x;
    int moveX = (int)round(remainder.x);
    if (moveX != 0) {
        remainder.x -= moveX;
        IRect playerRect = player->getRect();
        int signMove = Math::sign(moveX); // -1 or 1

        auto movePlayerX = [&] {
            while (moveX) {
                playerRect.pos.x += signMove;

                // exit out of the while loop if we hit a wall or solid tile
                IVec2 playerGridPos = world->get_grid_pos(player->position);
                for (int x = playerGridPos.x - 1; x <= playerGridPos.x + 1; x++) {
                    for (int y = playerGridPos.y - 1; y <= playerGridPos.y + 1; y++) {
                        Tile* tile = world->get_tile(x, y);
                        if (!tile || !tile->isVisible) {
                            continue;
                        }
                        FP_LOG("=== Collision Check ===");
                        FP_LOG("Tile: %d %d", x, y);
                        FP_LOG("Player Rect: %d %d %d %d", playerRect.pos.x, playerRect.pos.y, playerRect.size.x, playerRect.size.y);
                        FP_LOG("Tile Rect: %d %d %d %d", tile->getRect().pos.x, tile->getRect().pos.y, tile->getRect().size.x, tile->getRect().size.y);
                        FP_LOG("Tile Pos: %d %d", tile->position.x, tile->position.y);
                        if (Geometry::isColliding(playerRect, tile->getRect())) {
                            FP_LOG("Collision Detected");
                            player->speed.x = 0;
                            return;
                        }
                    }
                }

                // Move the Player
                player->position.x += signMove;
                moveX -= signMove;
            }
        };
        movePlayerX();
    }

    // Move the Player in Y until collision or moveY is exausted
    remainder.y += player->speed.y;
    int moveY = (int)round(remainder.y);
    if (moveY != 0) {
        remainder.y -= moveY;
        IRect playerRect = player->getRect();
        int signMove = Math::sign(moveY);
        auto movePlayerY = [&] {
            while (moveY) {
                playerRect.pos.y += signMove;
                // Loop through local Tiles
                IVec2 playerGridPos = world->get_grid_pos(player->position);
                for (int x = playerGridPos.x - 1; x <= playerGridPos.x + 1; x++) {
                    Tile* tile = world->get_tile(x, playerGridPos.y + 1);
                    if (!tile || !tile->isVisible) {
                        continue;
                    }
                    if (Geometry::isColliding(playerRect, tile->getRect())) {
                        player->speed.y = 0;
                        return;
                    }
                }
                // Move the Player
                player->position.y += signMove;
                moveY -= signMove;
            }
        };
        movePlayerY();
    }

    // Update the Camera position
    renderData->gameCamera.position = {(float)player->position.x, -(float)player->position.y};
}

void initializeGame()
{
    initializeKeybinds();

    // Store the location on the Spritesheet where the ground texture is located.
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
    renderData->gameCamera.position = {0, 0};
    gameState->initialized = true;
    FP_LOG("Game Initialized");
}

void executeGameTime(float deltaTime)
{
    handleInputActions(deltaTime);
    updatePlayerPosition();

    // Relative Mouse here, because more frames than simulations
    input->relMouse = input->mousePos - input->prevMousePos;
    input->prevMousePos = input->mousePos;

    // Finish our Game Tick by clearing the key presses
    for (int keyCode = 0; keyCode < KEY_COUNT; keyCode++) {
        input->keys[keyCode].justReleased = false;
        input->keys[keyCode].justPressed = false;
        input->keys[keyCode].halfTransitionCount = 0;
    }
    input->keys[KEY_MOUSE_SCROLL_DOWN].isDown = false;
    input->keys[KEY_MOUSE_SCROLL_UP].isDown = false;
}

void gameTickRunner(float deltaTime)
{
    gameState->internalTimer += deltaTime;

    // if/while
    while (gameState->internalTimer >= UPDATE_DELAY) {
        gameState->internalTimer -= (float)UPDATE_DELAY;
        gameState->tickCounter++;

        // FP_LOG("executeGameTime - %d", gameState->tickCounter);
        executeGameTime(deltaTime);
    }
}

// ###############################################
// #tag Functions Drawing
// ###############################################

void drawWorld()
{
    for (int y = 0; y < WORLD_GRID.y; y++) {
        for (int x = 0; x < WORLD_GRID.x; x++) {
            // Draw Background
            Transform bgTransformation = {};
            bgTransformation.pos = {x * (float)TILESIZE, y * (float)TILESIZE};
            bgTransformation.size = {16, 16};
            bgTransformation.spriteSize = {16, 16};
            // int randomTextureOffset[6] = {0, 0, 0, 16, 32, 48};
            // bgTransformation.atlasOffset = {192, randomTextureOffset[Math::random(0, 5)]};
            bgTransformation.atlasOffset = {192, 0};
            draw_quad(bgTransformation);
        }
    }
}

void drawCustomizableTileset()
{
    World* world = gameState->world;

    // Neighbouring Tiles
    int neighbourOffsets[24] = {
        0,  -1, // Top
        -1, 0,  // Left
        1,  0,  // Right
        0,  1,  // Bottom
        -1, -1, // Top-Left
        1,  -1, // Top-Right
        -1, 1,  // Bottom-Left
        1,  1,  // Bottom-Right
        0,  -2, // Top2
        -2, 0,  // Left2
        2,  0,  // Right2
        0,  2   // Bottom2
    };

    // Topleft     = BIT(4) = 16
    // Toplright   = BIT(5) = 32
    // Bottomleft  = BIT(6) = 64
    // Bottomright = BIT(7) = 128
    for (int y = 0; y < WORLD_GRID.y; y++) {
        for (int x = 0; x < WORLD_GRID.x; x++) {
            Tile* tile = world->get_tile(x, y);
            if (!tile->isVisible) {
                continue;
            }
            tile->neighbourMask = 0;
            int neighbourCount = 0;
            int extendedNeighbourCount = 0;
            int emptyNeighbourSlot = 0;
            // Look at the sorrounding 12 Neighbours
            for (int n = 0; n < 12; n++) {
                Tile* neighbour = world->get_tile(x + neighbourOffsets[n * 2], y + neighbourOffsets[n * 2 + 1]);
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

void drawTownFolk()
{
    draw_sprite(SPRITE_PLAYER, Vec2{32.0f, 32.0f});
    draw_sprite(SPRITE_PLAYER, Vec2{64.0f, 32.0f});
    draw_sprite(SPRITE_PLAYER, Vec2{32.0f, 64.0f});
    draw_sprite(SPRITE_PLAYER, Vec2{64.0f, 64.0f});
}

void drawPlayer()
{
    draw_sprite(SPRITE_PLAYER, gameState->player->position);
}

// ###############################################
// #tag Functions (exposed)
// ###############################################

EXPORT_FN void update_game(GameState* gameStateIn, RenderData* renderDataIn, Input* inputIn, float deltaTime)
{
    checkForHotReloadUpdates(gameStateIn, renderDataIn, inputIn);

    // Perform initialization logic
    if (!gameState->initialized) {
        initializeGame();
    }

    gameTickRunner(deltaTime);

    float interpolatedDT = (float)(gameState->internalTimer / UPDATE_DELAY);

    drawWorld();
    drawCustomizableTileset();
    drawTownFolk();
    drawPlayer();
}