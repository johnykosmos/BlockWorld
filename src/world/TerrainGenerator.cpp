#include "TerrainGenerator.hpp"
#include "utils/LCG.hpp"
#include <algorithm>
#include <iostream>

void TerrainGenerator::fillWater(Chunk* chunk) {
    for (int x = 0; x < CHUNK_SIZE_X; x++) {
        for (int z = 0; z < CHUNK_SIZE_Z; z++) {
            if (chunk->getBlock(x, SEA_TRESHOLD, z) == BlockID::Air) {
                chunk->setBlock(x, SEA_TRESHOLD, z, BlockID::Water);
            }
        }
    }
}

void TerrainGenerator::placeTree(ChunkBuildData& chunkData, iVec3 rootPosition, 
        uint chunkSeed) {  
    Chunk* chunk = chunkData.chunk;
    LCG lcg(chunkSeed + rand());
    uint height = static_cast<uint>(lcg.nextFloatInRange(
                                TREE_HEIGHT_MIN, TREE_HEIGHT_MAX));
    for (uint y = rootPosition.y; y <= rootPosition.y + height; y++) {
        if (y >= CHUNK_SIZE_Y) break;
        chunk->setBlock(rootPosition.x, y, rootPosition.z, BlockID::Log); 
    }

    int currentLayer = 0;
    int layerRange = 0;
    for (uint y = height; y < height + 4; y++) {
        currentLayer++;
        if (currentLayer % 3 == 0) layerRange++;
        for (int x = -2 + layerRange; x <= 2 - layerRange; x++) {
            for (int z = -2 + layerRange; z <= 2 - layerRange; z++) {
                int nX = rootPosition.x + x;
                int nY = rootPosition.y + y;
                int nZ = rootPosition.z + z;
                if (nX >= 0 && nX < CHUNK_SIZE_X && nY >= 0 && 
                        nY < CHUNK_SIZE_Y && nZ >= 0 && nZ < CHUNK_SIZE_Z) {
                    if (chunk->getBlock(nX, nY, nZ) == BlockID::Air) {
                        chunk->setBlock(nX, nY, nZ, BlockID::Leaves);
                    }
                } else {
                    int neighborX = (nX < 0) ? -1 : (nX >= CHUNK_SIZE_X ? 1 : 0);
                    int neighborZ = (nZ < 0) ? -1 : (nZ >= CHUNK_SIZE_Z ? 1 : 0);


                    ChunkCords chunkCords = chunk->getCords();
                    ChunkCords neighborCords = {
                        .x = chunkCords.x + neighborX,
                        .z = chunkCords.z + neighborZ
                    };
                    for (int i = 0; i < 4; i++) {
                        if (chunkData.chunkNeighbors[i] && 
                                chunkData.chunkNeighbors[i]->getCords() == neighborCords) {
                            nX = (nX + CHUNK_SIZE_X) % CHUNK_SIZE_X;
                            nZ = (nZ + CHUNK_SIZE_Z) % CHUNK_SIZE_Z; 
                            if (chunkData.chunkNeighbors[i]->getBlock(nX, nY, nZ) == BlockID::Air) {
                                chunkData.chunkNeighbors[i]->setBlock(nX, nY, nZ, BlockID::Leaves);
                            }
                        }
                    }
                }
            }
        }
    }
}

void TerrainGenerator::generateTerrain(Chunk* chunk, const Noise& noise) {
    ChunkCords chunkCords = chunk->getCords();
    for (int x = 0; x < CHUNK_SIZE_X; x++) {
        for (int z = 0; z < CHUNK_SIZE_Z; z++) {
            float worldX = float(chunkCords.x * CHUNK_SIZE_X + x);
            float worldZ = float(chunkCords.z * CHUNK_SIZE_Z + z);
            float baseValue = (noise.base->GetNoise(worldX, worldZ) + 1.0f) * 0.5f;

            float detailValue = (noise.detail->GetNoise(worldX, worldZ) + 1.0f) * 0.5f;

            float cliffZone = (detailValue > 0.7f) ? 1.0f : 0.0f;

            float curve = pow(baseValue, 1.5f);

            float finalValue = curve * detailValue;
            int height = int(finalValue * (CHUNK_SIZE_Y - 10));
            height = std::clamp(height, 0, CHUNK_SIZE_Y - 5);
            for (int y = height; y >= 0; y--) {
                BlockID block;
                if (y <= SEA_TRESHOLD + 1 && y >= SEA_TRESHOLD - 2) {
                    block = BlockID::Sand;
                } else if (y == height && y > SEA_TRESHOLD) {
                    block = BlockID::Grass;
                } else if (y >= height - 3) {
                    block = BlockID::Dirt;
                } else {
                    block = BlockID::Stone;
                }
                chunk->setBlock(x, y, z, block);
            }
        }
    }
    fillWater(chunk);
}

void TerrainGenerator::decorateTerrain(ChunkBuildData& chunkData, uint worldSeed) {
    Chunk* chunk = chunkData.chunk;
    ChunkCords chunkCords = chunk->getCords();
    uint chunkSeed = worldSeed + chunkCords.x * chunkCords.z;
    LCG lcg(chunkSeed);
    for (int x = 0; x < CHUNK_SIZE_X; x++) {
        for (int z = 0; z < CHUNK_SIZE_Z; z++) { 
            int y;
            for (y = CHUNK_SIZE_Y - 1; y >= 0; y--) {
                if (chunk->getBlock(x, y, z) != BlockID::Air) break;
            }

            if (chunk->getBlock(x, y, z) == BlockID::Grass) {
                if (lcg.nextFloat() < 0.002f) {
                    placeTree(chunkData, iVec3{x, y + 1, z}, chunkSeed);
                }
            }
        }
    }
    std::cout << "Decorating chunk at: " << chunkCords.x << " " << chunkCords.z << "\n";
}
