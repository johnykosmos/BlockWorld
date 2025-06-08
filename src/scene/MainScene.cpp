#include "MainScene.hpp"
#include "world/ChunkManager.hpp"

MainScene::MainScene() {
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
