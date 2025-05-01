#pragma once

#include <map>
#include <memory>
#include "Chunk.hpp"

using CordChunkMap = std::map<ChunkCords, std::unique_ptr<Chunk>>;

class ChunkManager {
    private:
        CordChunkMap loadedChunks;

    private: 
        bool loadMissingChunks(std::vector<ChunkCords>& missingChunks);
        bool unloadNotUsedChunks(std::vector<ChunkCords>& neededChunks);

    public:
        const CordChunkMap& getLoadedChunks() const;
        bool updateChunks(Vec3 playerPosition, int renderDistance);
};
