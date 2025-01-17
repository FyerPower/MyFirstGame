#pragma once

#include "libs/generic_types.hpp"
#include "libs/geometry.hpp"

class OrthographicCamera2D
{
  public:
    float zoom = 1.0f;
    Vec2 dimensions;
    Vec2 position;

    Vec2 getZoomedDimensions()
    {
        return dimensions / zoom;
    }

    Mat4 getOrthographicProjection()
    {
        Vec2 zoomedDimensions = getZoomedDimensions();
        return Geometry::orthographic_projection(position.x - zoomedDimensions.x / 2,  // left
                                                 position.x + zoomedDimensions.x / 2,  // right
                                                 position.y + zoomedDimensions.y / 2,  // top
                                                 position.y - zoomedDimensions.y / 2); // bottom
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