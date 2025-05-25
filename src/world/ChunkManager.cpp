#include "ChunkManager.hpp"
#include "world/Chunk.hpp"
#include <iostream>
#include <memory>


const CordChunkMap& ChunkManager::getLoadedChunks() const {
    return loadedChunks;
}

const Chunk* ChunkManager::getChunk(ChunkCords position) {
    auto it = loadedChunks.find(position);
    if (it != loadedChunks.end()) {
        return loadedChunks[position].get();
    }
    return nullptr;
}

bool ChunkManager::loadMissingChunks(std::vector<ChunkCords>& missingChunks) {
    bool updated = false;
    for (const auto& missingCords : missingChunks) {
        if(getChunk(missingCords) && loadedChunks[missingCords]->isDirty()) {
            const ChunkBuildData data = {
                .chunk = loadedChunks[missingCords].get(),
                .chunkNeighbors = {
                    getChunk(ChunkCords{missingCords.x + 1, missingCords.z}),
                    getChunk(ChunkCords{missingCords.x - 1, missingCords.z}),
                    getChunk(ChunkCords{missingCords.x, missingCords.z + 1}),
                    getChunk(ChunkCords{missingCords.x, missingCords.z - 1}),
                }
            };
            chunkBuilder.enqueueChunk(data);
            updated = true;
            std::cout << "Loaded a chunk at: " << missingCords.x << " " << missingCords.z << "\n";
        }
    }
    chunkBuilder.pollFinishedChunks();    
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
                .x = (int)playerPosition.x/CHUNK_SIZE_X + x,
                .z = (int)playerPosition.z/CHUNK_SIZE_Z + z
            };
            if (!getChunk(wantedCords)) {
                loadedChunks[wantedCords] = std::make_unique<Chunk>(wantedCords);
            }
            wantedChunks.push_back(wantedCords);
        }
    }

    bool loaded = loadMissingChunks(wantedChunks);
    bool unloaded = unloadNotUsedChunks(wantedChunks);

    return loaded || unloaded;
}

