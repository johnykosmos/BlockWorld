#pragma once

#include <map>
#include <memory>
#include "Chunk.hpp"
#include "world/ChunkBuilder.hpp"

using CordChunkMap = std::map<ChunkCords, std::unique_ptr<Chunk>>;


class ChunkManager {
    private:
        CordChunkMap loadedChunks;
        ChunkBuilder chunkBuilder = ChunkBuilder(4);

    private:
        const Chunk* getChunk(ChunkCords position);
        bool loadMissingChunks(std::vector<ChunkCords>& missingChunks);
        bool unloadNotUsedChunks(std::vector<ChunkCords>& neededChunks);
        

    public:
        const CordChunkMap& getLoadedChunks() const;
        bool updateChunks(Vec3 playerPosition, int renderDistance);
};



