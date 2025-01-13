# Inventory Management:

```cpp
struct Item {
  char* id;
  int amount;

  // Use Localization
  char* name() { return "ItemName"; }

  // Remove
  void decreaseAmount(int amountToDecrease) { amount -= amountToDecrease }
}

struct WorldItem {
    Item item;
    Position position;

    bool withinPickupRange(Position pos) {
        if(distance(position, pos) < 1) {
            return true;
        }
        return false;
    }
}

struct Inventory {
  int gold;
  std::unordered_map<int, Item> items;

  int count() { return items.size() }
  bool hasItem(char* itemId, int amount) {

  }
  Item getSlot(int slotId) {
    return items[slotId];
  }
  void clearSlot(int slotId) {
    items[slotId] = NULL;
  }
  void clearAll() {}
  bool removeItem(char* itemId, int amount) {
    if(!hasItem(itemId, amount)) {
      return false;
    }
    // remove items
    return true;
  }
}
```

# Player

# Townie

# Block

# Neighboring Logic

```cpp
void initializeTileset()
{
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
```
