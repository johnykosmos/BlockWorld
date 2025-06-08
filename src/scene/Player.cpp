#include "Player.hpp"
#include "Camera.hpp"
#include "Engine.hpp"


Player::Player(eng::PerspectiveCamera& camera) : camera(camera) {}

void Player::update(){
    eng::Input input = eng::Engine::getInstance().getInputHandler();
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

const Vec3 Player::getPosition() const {
    return camera.getPosition();
}
