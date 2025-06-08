#include "MainScene.hpp"
#include "world/ChunkManager.hpp"
#include <cstdlib>

MainScene::MainScene() : chunkMng(rand()), player(camera, chunkMng) {
    setActiveCamera(camera);
    addLight(baseLight);
}

void MainScene::onUpdate() {
    renderer.clear();
    renderer.clearToColor(Vec3(0.541f, 0.725f, 0.945f));
    
    if (chunkMng.updateChunks(player.getPosition(), RENDER_DISTANCE)) {
        const CordChunkMap& chunkMap = chunkMng.getLoadedChunks();

        clearDrawables();
        for (const auto& [cords, chunk] : chunkMap) {
            addDrawable(*chunk, material);
        }
    }

    player.update(); 
}
