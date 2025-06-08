#pragma once

#include "Typedefs.hpp"

#define CUBE_FACES 6
#define VERTICES_PER_FACE 4
#define INDICES_PER_FACE 6

constexpr float BASE_UV = 1.0f/4;

struct Face {
    Vec3 position[VERTICES_PER_FACE];
    Vec3 normal;
};

extern Face cubeFaces[CUBE_FACES];
extern uint cubeFaceIndices[INDICES_PER_FACE];
extern Vec2 textureUV[VERTICES_PER_FACE];
