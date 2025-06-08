#include "ChunkManager.hpp"
#include "world/Chunk.hpp"
#include <iostream>
#include <set>
#include <vector>


const ChunkCords chunkNeighbors[4] = {
    {1,0},{-1,0},{0,1},{0,-1}
};

ChunkManager::ChunkManager(uint seed) {
    baseNoise.SetSeed(seed);
    baseNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    baseNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
    baseNoise.SetFractalOctaves(4);
    baseNoise.SetFrequency(0.001f);

    detailNoise.SetSeed(seed + 100);
    detailNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    detailNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
    detailNoise.SetFractalOctaves(3);
    detailNoise.SetFrequency(0.01f);

    const Noise noises = {
        .base = &baseNoise,
        .detail = &detailNoise
    };

    chunkBuilder = std::make_unique<ChunkBuilder>(
            std::thread::hardware_concurrency(), seed, noises);
}

const CordChunkMap& ChunkManager::getLoadedChunks() const {
    return loadedChunks;
}

Chunk* ChunkManager::getChunk(ChunkCords position) {
    auto it = loadedChunks.find(position);
    if (it != loadedChunks.end()) {
        return loadedChunks[position].get();
    }
    return nullptr;
}

bool ChunkManager::loadMissingChunks(std::set<ChunkCords>& missingChunks) {
    bool updated = false;
    std::vector<ChunkCords> newChunks;
    std::vector<ChunkCords> chunksToRebuild;
    for (const auto& missingCords : missingChunks) {
        if (!getChunk(missingCords)) {
            loadedChunks[missingCords] = std::make_unique<Chunk>(missingCords);
            newChunks.push_back(missingCords);
        } else {
            chunksToRebuild.push_back(missingCords);
        }
    }

    for (auto& newChunkCords : newChunks) {
        const ChunkBuildData data = {
                .chunk = loadedChunks[newChunkCords].get(),
                .chunkNeighbors = {
                    getChunk(ChunkCords{newChunkCords.x + 1, newChunkCords.z}),
                    getChunk(ChunkCords{newChunkCords.x - 1, newChunkCords.z}),
                    getChunk(ChunkCords{newChunkCords.x, newChunkCords.z + 1}),
                    getChunk(ChunkCords{newChunkCords.x, newChunkCords.z - 1}),
                },
                .isNew = true
            };
            chunkBuilder->enqueueChunk(data);
            updated = true;
            std::cout << "Loaded a chunk at: " << newChunkCords.x << " " << newChunkCords.z << "\n";
    }

    for (auto& toRebuildCords : chunksToRebuild) {

        const ChunkBuildData data = {
            .chunk = loadedChunks[toRebuildCords].get(),
            .chunkNeighbors = {
                getChunk(ChunkCords{toRebuildCords.x + 1, toRebuildCords.z}),
                getChunk(ChunkCords{toRebuildCords.x - 1, toRebuildCords.z}),
                getChunk(ChunkCords{toRebuildCords.x, toRebuildCords.z + 1}),
                getChunk(ChunkCords{toRebuildCords.x, toRebuildCords.z - 1}),
            },
            .isNew = false
        };
        chunkBuilder->enqueueChunk(data);
        updated = true;
        std::cout << "Rebuilt a chunk at: " << toRebuildCords.x << " " << toRebuildCords.z << "\n";
    }

    chunkBuilder->pollFinishedChunks();    
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
    std::set<ChunkCords> newChunks;

    for (int x = -renderDistance; x < renderDistance; x++) {
        for (int z = -renderDistance; z < renderDistance; z++) {
            ChunkCords wantedCords = {
                .x = (int)playerPosition.x/CHUNK_SIZE_X + x,
                .z = (int)playerPosition.z/CHUNK_SIZE_Z + z
            };
            
            if (!getChunk(wantedCords)) {
                newChunks.insert(wantedCords);
                for (int i = 0; i < 4; i++) {
                    ChunkCords neighborCords = wantedCords + chunkNeighbors[i];
                    Chunk* neighborChunk = getChunk(neighborCords);              
                    if (neighborChunk) {
                        newChunks.insert(neighborCords);
                    }
                }
            }

            wantedChunks.push_back(wantedCords);
        }
    }


    bool loaded = loadMissingChunks(newChunks);
    bool unloaded = unloadNotUsedChunks(wantedChunks);

    return loaded || unloaded;
}

