#pragma once

#include <map>
#include <memory>
#include <set>
#include "FastNoiseLite.h"
#include "world/ChunkBuilder.hpp"

using CordChunkMap = std::map<ChunkCords, std::unique_ptr<Chunk>>;


class ChunkManager {
    private:
        FastNoiseLite baseNoise;
        FastNoiseLite detailNoise;
        CordChunkMap loadedChunks;
        std::unique_ptr<ChunkBuilder> chunkBuilder; 

    private:
        Chunk* getChunk(ChunkCords position);
        bool loadMissingChunks(std::set<ChunkCords>& missingChunks);
        bool unloadNotUsedChunks(std::vector<ChunkCords>& neededChunks);
        

    public:
        ChunkManager(unsigned int seed);
        const CordChunkMap& getLoadedChunks() const;
        bool updateChunks(Vec3 playerPosition, int renderDistance);
};



