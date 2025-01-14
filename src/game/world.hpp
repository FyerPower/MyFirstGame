#pragma once

#include "../libs/libs.hpp"
#include "config.hpp"
#include "sprite.hpp"

class Tile
{
  public:
    Sprite* sprite;
    IVec2 position;

    Tile()
    {}
    Tile(int x, int y)
    {
        FP_ASSERT(false, "Tile Created without Parameters");
    }
    Tile(int x, int y, Sprite* sprite = nullptr)
    {
        this->position = {x, y};
        this->sprite = sprite;
    }

    IVec2 getWorldPos()
    {
        return {this->position.x * TILESIZE, this->position.y * TILESIZE};
    }

    std::optional<IRect> getHitbox()
    {
        auto hitbox = this->sprite->getHitbox();
        if (hitbox.has_value()) {
            return hitbox.value() + this->getWorldPos();
        }
        return std::nullopt;
    }
};

class World
{
  public:
    Tile* worldGrid[WORLD_GRID.x][WORLD_GRID.y];

    World()
    {}

    void setTile(int x, int y, Sprite* sprite)
    {
        this->worldGrid[x][y] = new Tile(x, y, sprite);
    }

    Tile* get_tile(int x, int y)
    {
        if (x >= 0 && x < WORLD_GRID.x && y >= 0 && y < WORLD_GRID.y) {
            return this->worldGrid[x][y];
        }
        return nullptr;
    }

    Tile* get_tile(IVec2 worldPos)
    {
        int x = worldPos.x / TILESIZE;
        int y = worldPos.y / TILESIZE;
        return get_tile(x, y);
    }

    IVec2 get_grid_pos(IVec2 worldPos)
    {
        return {worldPos.x / TILESIZE, worldPos.y / TILESIZE};
    }
};
