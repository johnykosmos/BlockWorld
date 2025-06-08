#include "MainScene.hpp"
#include "Engine.hpp"
#include "world/ChunkManager.hpp"

MainScene::MainScene() {
    setActiveCamera(camera);
    addLight(baseLight);
    //renderer.enableLighting(false);
}

void MainScene::updateCamera(){
    eng::Input input = engine.getInputHandler();
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

void MainScene::onUpdate() {
    renderer.clear();
    renderer.clearToColor(Vec3(0.541f, 0.725f, 0.945f));
    
    if (chunkMng.updateChunks(camera.getPosition(), RENDER_DISTANCE)) {
        const CordChunkMap& chunkMap = chunkMng.getLoadedChunks();

        clearDrawables();
        for (const auto& [cords, chunk] : chunkMap) {
            addDrawable(*chunk, material);
        }
    }

    updateCamera(); 
}
