#pragma once

#include "world/Chunk.hpp"
#include "FastNoiseLite.h"


#define SEA_TRESHOLD 7
#define TREE_HEIGHT_MIN 2.0f
#define TREE_HEIGHT_MAX 4.0f

/**
 * @struct Noise
 * @brief Holds references to base and detail noise generators for procedural terrain generation.
 */
struct Noise {
    /**
     * @brief Pointer to the base noise generator (e.g., for elevation).
     */
    const FastNoiseLite* base;

    /**
     * @brief Pointer to the detail noise generator (e.g., for fine surface variation).
     */
    const FastNoiseLite* detail;
};

/**
 * @struct ChunkBuildData
 * @brief Contains information needed to build or decorate a chunk, including neighbor data.
 */
struct ChunkBuildData {
    /**
     * @brief Pointer to the chunk being built.
     */
    Chunk* chunk;

    /**
     * @brief Array of pointers to the four neighboring chunks (N, E, S, W).
     */
    Chunk* chunkNeighbors[4];

    /**
     * @brief Flag indicating whether the chunk is newly generated.
     */
    bool isNew;
};

/**
 * @class TerrainGenerator
 * @brief Static utility class responsible for generating and decorating terrain chunks.
 * 
 * Includes functions for populating terrain using noise data and placing features such as trees and water.
 */
class TerrainGenerator {
private:
    /**
     * @brief Fills a chunk with water blocks below a certain height threshold.
     * @param chunk Pointer to the chunk to fill with water.
     */
    static void fillWater(Chunk* chunk);

    /**
     * @brief Places a tree at the given root position within a chunk.
     * @param chunkData Reference to the build data for the chunk.
     * @param rootPosition The position where the tree root will be placed.
     * @param chunkSeed The seed value specific to this chunk, used for randomness.
     */
    static void placeTree(ChunkBuildData& chunkData, iVec3 rootPosition, uint chunkSeed);

public:
    /**
     * @brief Generates terrain in the specified chunk using provided noise data.
     * @param chunk Pointer to the chunk to be populated with terrain.
     * @param noise Struct containing noise generator references.
     */
    static void generateTerrain(Chunk* chunk, const Noise& noise);

    /**
     * @brief Decorates the terrain with features like trees or rocks after terrain generation.
     * @param chunkData Reference to the chunk build data.
     * @param worldSeed Seed value used to ensure consistent decoration across worlds.
     */
    static void decorateTerrain(ChunkBuildData& chunkData, uint worldSeed);
};

