#pragma once

#include "../libs/array.hpp"
#include "../camera.hpp"
#include "transform.hpp"

// #############################################################################
// #tag Structs
// #############################################################################

// A store for all of our Rendered Objects
class RenderData
{
  public:
    Camera gameCamera;
    Camera uiCamera;

    Array<Transform, 1000> transforms; // Transformation Store
    // Array<Material, 1000> materials;   // Transformation Store
};

// #############################################################################
// #tag Globals
// #############################################################################

static RenderData* renderData;