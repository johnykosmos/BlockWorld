#include "ChunkManager.hpp"
#include "world/Chunk.hpp"
#include <iostream>



ChunkManager::ChunkManager(unsigned int seed) {
    baseNoise.SetSeed(seed);
    baseNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    baseNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
    baseNoise.SetFractalOctaves(4);
    baseNoise.SetFrequency(0.005f);

    detailNoise.SetSeed(seed + 2);
    detailNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    detailNoise.SetFractalType(FastNoiseLite::FractalType_None);
    detailNoise.SetFrequency(0.04f);

    const Noise noises = {
        .base = &baseNoise,
        .detail = &detailNoise
    };

    chunkBuilder = std::make_unique<ChunkBuilder>(
            std::thread::hardware_concurrency(), noises);
}

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

bool ChunkManager::loadMissingChunks(std::set<ChunkCords>& missingChunks) {
    bool updated = false;
    for (const auto& missingCords : missingChunks) {
        if(!getChunk(missingCords)) {
            loadedChunks[missingCords] = std::make_unique<Chunk>(missingCords);
            const ChunkBuildData data = {
                .chunk = loadedChunks[missingCords].get(),
                .chunkNeighbors = {
                    getChunk(ChunkCords{missingCords.x + 1, missingCords.z}),
                    getChunk(ChunkCords{missingCords.x - 1, missingCords.z}),
                    getChunk(ChunkCords{missingCords.x, missingCords.z + 1}),
                    getChunk(ChunkCords{missingCords.x, missingCords.z - 1}),
                },
                .isNew = true
            };
            chunkBuilder->enqueueChunk(data);
            updated = true;
            std::cout << "Loaded a chunk at: " << missingCords.x << " " << missingCords.z << "\n";
        }
    }
    chunkBuilder->pollFinishedChunks();    
    return updated;
}


bool ChunkManager::unloadNotUsedChunks(std::set<ChunkCords>& neededChunks) {
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
    std::set<ChunkCords> wantedChunks;

    for (int x = -renderDistance; x < renderDistance; x++) {
        for (int z = -renderDistance; z < renderDistance; z++) {
            ChunkCords wantedCords = {
                .x = (int)playerPosition.x/CHUNK_SIZE_X + x,
                .z = (int)playerPosition.z/CHUNK_SIZE_Z + z
            };
            
            wantedChunks.insert(wantedCords);
        }
    }

    bool loaded = loadMissingChunks(wantedChunks);
    bool unloaded = unloadNotUsedChunks(wantedChunks);

    return loaded || unloaded;
}

