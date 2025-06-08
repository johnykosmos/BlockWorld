#pragma once

#include "world/Chunk.hpp"
#include <vector>


struct Noise {
    const FastNoiseLite* base;
    const FastNoiseLite* detail;
};

struct ChunkBuildData {
    Chunk* chunk;
    Chunk* chunkNeighbors[4];
    bool isNew;
};

class TerrainGenerator {
    private:
        static void fillWater(Chunk* chunk);
        static void placeTree(ChunkBuildData& chunkData, iVec3 rootPosition, 
                uint chunkSeed);

    public:
        static void generateTerrain(Chunk* chunk, const Noise& noise);
        static void decorateTerrain(ChunkBuildData& chunkData, uint worldSeed);

};
