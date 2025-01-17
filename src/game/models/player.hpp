#pragma once

#include "config.hpp"
#include "shared/libs/libs.hpp"

class Player
{
  public:
    IVec2 position;
    IVec2 previousPosition;
    IVec2 size = {16, 32};
    Vec2 speed;
    int health;
    int maxHealth = 60;
    int stamina;
    int maxStamina = 100;

    IRect getHitbox()
    {
        return {{this->position.x - 5, this->position.y - 2}, {10, 10}};
    }
    void setSpeedX(int sign)
    {
        // Multiplier is 3.0f if we are changing direction
        float mult = (sign > 0 && speed.y < 0 || sign < 0 && speed.y > 0) ? 3.0f : 1.0f;
        speed.x = Math::approach(speed.x, sign * runSpeed, runAcceleration * mult * (float)UPDATE_DELAY);
    }
    void setSpeedY(int sign)
    {
        // Multiplier is 3.0f if we are changing direction
        float mult = (sign > 0 && speed.y < 0 || sign < 0 && speed.y > 0) ? 3.0f : 1.0f;
        speed.y = Math::approach(speed.y, sign * runSpeed, runAcceleration * mult * (float)UPDATE_DELAY);
    }

  private:
    bool isGrounded = true;
    float runSpeed = 1.0f;
    float runAcceleration = 10.0f;
    float gravity = 13.0f;
    float fallSpeed = 3.6f;
    float jumpSpeed = -3.0f;
};