#include "ChunkManager.hpp"
#include "world/Chunk.hpp"
#include <iostream>
#include <memory>


const CordChunkMap& ChunkManager::getLoadedChunks() const {
    return loadedChunks;
}

bool ChunkManager::loadMissingChunks(std::vector<ChunkCords>& missingChunks) {
    bool updated = false;
    for (const auto& missingCords : missingChunks) {
        auto it = loadedChunks.find(missingCords);
        if (it == loadedChunks.end()) {
            loadedChunks[missingCords] = std::make_unique<Chunk>(missingCords);
            loadedChunks[missingCords]->buildMesh();
            updated = true;
            std::cout << "Loaded a chunk at: " << missingCords.x << " " << missingCords.z << "\n";
        }
    }
    return updated;
}


bool ChunkManager::unloadNotUsedChunks(std::vector<ChunkCords>& neededChunks) {
    bool updated = false;
    std::vector<ChunkCords> toErase;
    for (const auto& [cords, chunk] : loadedChunks) {
        bool found = false;
        for (const auto& neededCords : neededChunks) {
            if (neededCords == cords) {
                found = true;
                break;
            }
        }
        if (!found) {
            toErase.push_back(cords);
            updated = true;
        }
    }

    for (const auto& cords : toErase) {
        loadedChunks.erase(cords);
    }
    return updated;
}

bool ChunkManager::updateChunks(Vec3 playerPosition, 
        int renderDistance) {
    std::vector<ChunkCords> wantedChunks;

    for (int x = -renderDistance; x < renderDistance; x++) {
        for (int z = -renderDistance; z < renderDistance; z++) {
            ChunkCords wantedCords = {
                .x = (int)playerPosition.x/16 + x,
                .z = (int)playerPosition.z/16 + z
            };

            wantedChunks.push_back(wantedCords);
        }
    }

    bool loaded = loadMissingChunks(wantedChunks);
    bool unloaded = unloadNotUsedChunks(wantedChunks);

    return loaded || unloaded;
}

