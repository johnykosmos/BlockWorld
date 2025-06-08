#include "Player.hpp"
#include "Camera.hpp"
#include "world/BlockAtlas.hpp"
#include "world/ChunkManager.hpp"
#include <iostream>


Player::Player(eng::PerspectiveCamera& camera, ChunkManager& chunkMng) : 
    camera(camera), chunkMng(chunkMng) {}


bool Player::raycastBlock(iVec3& hitPosition) {
    Vec3 front = camera.getFront();
    Vec3 currentPosition = camera.getPosition();
    for (int i = 0; i < PLAYER_RANGE; i++) {
        currentPosition += front;           
        iVec3 blockPos = {
            static_cast<int>(currentPosition.x),
            static_cast<int>(currentPosition.y),
            static_cast<int>(currentPosition.z)
        };

        BlockID block = chunkMng.getBlockAt(blockPos);
        if (block != BlockID::Air && block != BlockID::Water) {
            hitPosition = blockPos;
            return true;
        }
    }
    return false;
}

void Player::destroyBlock() {
    iVec3 hitPos;
    if (raycastBlock(hitPos)) {
        chunkMng.replaceBlock(hitPos, BlockID::Air);

        std::cout << "Hit a block at: " << hitPos.x << " " << hitPos.y << " " << hitPos.z << "\n";
    }

}

void Player::updateCamera() {
    auto cameraPosition = camera.getPosition();
    if (input.isKeyPressed(eng::Key::Up)){
        camera.setPitch(camera.getPitch() + 1);
    }if (input.isKeyPressed(eng::Key::Down)){
        camera.setPitch(camera.getPitch() - 1);
    }if (input.isKeyPressed(eng::Key::Right)){
        camera.setYaw(camera.getYaw() + 1);
    }if (input.isKeyPressed(eng::Key::Left)){
        camera.setYaw(camera.getYaw() - 1);
    }if (input.isKeyPressed(eng::Key::W)){
        cameraPosition += camera.getFront() * 0.3f;
    }if (input.isKeyPressed(eng::Key::A)){
        cameraPosition -= camera.getRight() * 0.3f;
    }if (input.isKeyPressed(eng::Key::S)){
        cameraPosition -= camera.getFront() * 0.3f;
    }if (input.isKeyPressed(eng::Key::D)){
        cameraPosition += camera.getRight() * 0.3f;
    }

    camera.setPosition(cameraPosition);
    //std::cout << "Camera Position: " << cameraPosition.x << " " << cameraPosition.y << " " << cameraPosition.z << std::endl;
}

void Player::update() {
    updateCamera();
    if (input.isKeyPressed(eng::Key::Space)) {
        destroyBlock();
    }
}

const Vec3 Player::getPosition() const {
    return camera.getPosition();
}
