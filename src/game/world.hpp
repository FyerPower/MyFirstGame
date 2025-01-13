#pragma once

#include "config.hpp"
#include "../libs/libs.hpp"

class Tile
{
  public:
    Sprite* sprite;
    IVec2 position;

    Tile()
    {
        Logger::asssert(false, "Tile Created without Parameters");
    }
    Tile(int x, int y, Sprite* sprite = nullptr)
    {
        this->position = {x, y};
        this->sprite = sprite;
    }

    IVec2 getPos()
    {
        return {this->position.x * TILESIZE, this->position.y * TILESIZE};
    }

    IRect getHitbox()
    {
        return this->sprite->hitbox + this->getPos();
        // return {this->getPos(), {TILESIZE, TILESIZE}};
    }
};

class World
{
  public:
    Tile* worldGrid[WORLD_GRID.x][WORLD_GRID.y];
    // Object worldGrid[WORLD_GRID.x][WORLD_GRID.y];
    // Entity worldGrid[WORLD_GRID.x][WORLD_GRID.y];

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
