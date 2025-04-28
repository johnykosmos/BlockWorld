#include "ChunkManager.hpp"
#include "world/Chunk.hpp"
#include <iostream>
#include <memory>


const CordChunkMap& ChunkManager::getLoadedChunks() const {
    return loadedChunks;
}

bool ChunkManager::updateChunks(Vec3 playerPosition, 
        int renderDistance) {
    bool updated = false;
    std::vector<ChunkCords> wantedChunks;

    for (int x = -renderDistance; x < renderDistance; x++) {
        for (int z = -renderDistance; z < renderDistance; z++) {
            ChunkCords wantedCords = {
                .x = (int)playerPosition.x/16 + x,
                .z = (int)playerPosition.z/16 + z
            };

            wantedChunks.push_back(wantedCords);
            auto it = loadedChunks.find(wantedCords);
            if (it == loadedChunks.end()) {
                loadedChunks[wantedCords] = std::make_unique<Chunk>(wantedCords);
                loadedChunks[wantedCords]->buildMesh();
                updated = true;
                std::cout << "Loaded a chunk at: " << wantedCords.x << " " << wantedCords.z << "\n";
            } 
        }
    }

    if (updated) {
        std::vector<ChunkCords> toErase;

        for (const auto& [cords, chunk] : loadedChunks) {
            bool found = false;
            for (const auto& wanted : wantedChunks) {
                if (wanted == cords) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                toErase.push_back(cords);
            }
        }

        for (const auto& cords : toErase) {
            loadedChunks.erase(cords);
        }
    }

    return updated;
}

