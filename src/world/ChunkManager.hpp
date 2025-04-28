#pragma once

#include <map>
#include <memory>
#include "Chunk.hpp"

using CordChunkMap = std::map<ChunkCords, std::unique_ptr<Chunk>>;

class ChunkManager {
    private:
        CordChunkMap loadedChunks;

    public:
        const CordChunkMap& getLoadedChunks() const;
        bool updateChunks(Vec3 playerPosition, int renderDistance);
};
