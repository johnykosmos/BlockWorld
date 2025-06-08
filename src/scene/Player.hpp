#pragma once 

#include "Engine.hpp"
#include "Camera.hpp"
#include "Typedefs.hpp"
#include "world/ChunkManager.hpp"

#define PLAYER_RANGE 5

class Player {
    private:
        eng::PerspectiveCamera& camera;
        eng::Input input = eng::Engine::getInstance().getInputHandler();
        ChunkManager& chunkMng;

    private:
        bool raycastBlock(iVec3& hitPosition);
        void destroyBlock();
        void updateCamera();

    public:
        Player(eng::PerspectiveCamera& camera, ChunkManager& chunkMng);
        const Vec3 getPosition() const;
        void update();

};
