#pragma once

#include "shared/libs/constants.hpp"
#include "shared/libs/generic_types.hpp"
#include "shared/libs/geometry.hpp"

class Camera
{
  public:
    float zoom = 1.0f;
    Vec2 dimensions;
    Vec2 position;

    Vec2 getZoomedDimensions()
    {
        return dimensions / zoom;
    }

    Mat4 getOrthographicProjection(IVec2 screenSize, bool clampToWorld = true)
    {
        Vec2 zoomedDimensions = getZoomedDimensions();
        float left = position.x - zoomedDimensions.x / 2;
        float right = position.x + zoomedDimensions.x / 2;
        float top = position.y + zoomedDimensions.y / 2;
        float bottom = position.y - zoomedDimensions.y / 2;

        if (clampToWorld) {
            // if (left < 0) {
            //     left = 0;
            //     right = right + screenSize.x;
            // } else if (right > WORLD_WIDTH) {
            //     right = WORLD_WIDTH;
            //     left = left - screenSize.x;
            // }
            // if (top < 0) {
            //     top = 0.0;
            //     bottom = (float)screenSize.y;
            // } else if (bottom > WORLD_HEIGHT) {
            //     bottom = (float)WORLD_HEIGHT;
            //     top = bottom - screenSize.y;
            // }
        }

        return Geometry::orthographic_projection(left, right, bottom, top);
    }

    void zoomIn()
    {
        zoom += zoom_amount;
        if (zoom > zoom_max) {
            zoom = zoom_max;
        }
    }
    void zoomOut()
    {
        zoom -= zoom_amount;
        if (zoom < zoom_min) {
            zoom = zoom_min;
        }
    }

  private:
    float zoom_min = 0.5f;
    float zoom_max = 2.0f;
    float zoom_amount = 0.1f;
};