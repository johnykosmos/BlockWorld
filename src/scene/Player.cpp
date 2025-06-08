#include "Player.hpp"
#include "Camera.hpp"
#include "Event.hpp"
#include "Keys.hpp"
#include "world/BlockAtlas.hpp"
#include "world/ChunkManager.hpp"


Player::Player(eng::PerspectiveCamera& camera, ChunkManager& chunkMng) : 
    camera(camera), chunkMng(chunkMng) {}


bool Player::raycastBlock(iVec3& hitPosition, iVec3& placePosition) {
    Vec3 ray = camera.getFront() * 0.2f;
    Vec3 currentPosition = camera.getPosition();
    for (int i = 0; i < PLAYER_RANGE / 0.2f; i++) {
        placePosition = currentPosition;
        currentPosition += ray;           
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

void Player::placeBlock() {
    iVec3 hitPos, placePos;
    Vec3 front = camera.getFront();
    if (raycastBlock(hitPos, placePos)) {
        BlockID block = chunkMng.getBlockAt(placePos);
        if (block == BlockID::Air || block == BlockID::Water) {
            chunkMng.replaceBlock(placePos, BlockID::Dirt);               
        }
    }
}

void Player::destroyBlock() {
    iVec3 hitPos, placePos;
    if (raycastBlock(hitPos, placePos)) {
        chunkMng.replaceBlock(hitPos, BlockID::Air);
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
    eng::Event event;
    while (input.pollEvents(event)) {
        if (event.getEventType() == eng::EventType::KeyPressedEvent) { 
            if (event.getKey().keyCode == eng::Key::Space) {
                destroyBlock();
            } else if (event.getKey().keyCode == eng::Key::Enter) {
                placeBlock();
            }
        }
    }
}

const Vec3 Player::getPosition() const {
    return camera.getPosition();
}
