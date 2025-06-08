#include "CubeRenderParameters.hpp"

Face cubeFaces[CUBE_FACES] = {
    // Right
    {
        .position = { {1,0,0}, {1,1,0}, {1,1,1}, {1,0,1} },
        .normal = {1,0,0}
    },
    // Left
    {
        .position = { {0,0,1}, {0,1,1}, {0,1,0}, {0,0,0} },
        .normal = {-1,0,0}
    },
    // Front
    {
        .position = { {1,0,1}, {1,1,1}, {0,1,1}, {0,0,1} },
        .normal = {0,0,1}
    },
    // Back
    {
        .position = { {0,0,0}, {0,1,0}, {1,1,0}, {1,0,0} },
        .normal = {0,0,-1}
    },
    // Top
    {
        .position = { {0,1,0}, {0,1,1}, {1,1,1}, {1,1,0} },
        .normal = {0,1,0}
    },
    // Bottom
    {
        .position = { {0,0,1}, {0,0,0}, {1,0,0}, {1,0,1} },
        .normal = {0,-1,0}
    }
};

uint cubeFaceIndices[INDICES_PER_FACE] = {
    0, 1, 2,
    0, 2, 3
};

Vec2 textureUV[VERTICES_PER_FACE] = {
    {0.0f, BASE_UV},
    {0.0f, 0.0f}, 
    {BASE_UV, 0.0f},     
    {BASE_UV, BASE_UV}, 
};
