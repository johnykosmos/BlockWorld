#pragma once

#include <map>
#include <memory>
#include <set>
#include "FastNoiseLite.h"
#include "world/ChunkBuilder.hpp"

using CordChunkMap = std::map<ChunkCords, std::unique_ptr<Chunk>>;



/**
 * @class ChunkManager
 * @brief Manages the loading, unloading, generation, and access of world chunks.
 * 
 * Utilizes procedural noise and a chunk builder to generate and update chunks dynamically based on player position.
 */
class ChunkManager {
    private:
        /**
         * @brief Base noise generator for terrain shaping (e.g., elevation).
         */
        FastNoiseLite baseNoise;

        /**
         * @brief Detail noise generator for fine-grained terrain features.
         */
        FastNoiseLite detailNoise;

        /**
         * @brief A map of currently loaded chunks indexed by their coordinates.
         */
        CordChunkMap loadedChunks;

        /**
         * @brief Responsible for constructing chunk data, possibly including mesh generation.
         */
        std::unique_ptr<ChunkBuilder> chunkBuilder;

        /**
         * @brief Retrieves a pointer to the chunk at a given position.
         * @param position The chunk coordinates to retrieve.
         * @return Pointer to the Chunk, or nullptr if not loaded.
         */
        Chunk* getChunk(ChunkCords position);

        /**
         * @brief Loads chunks that are missing from the current set of loaded chunks.
         * @param missingChunks Set of chunk coordinates that need to be loaded.
         * @return true if any chunks were loaded, false otherwise.
         */
        bool loadMissingChunks(std::set<ChunkCords>& missingChunks);

        /**
         * @brief Unloads chunks that are no longer within the area of interest.
         * @param neededChunks Vector of currently needed chunk coordinates.
         * @return true if any chunks were unloaded, false otherwise.
         */
        bool unloadNotUsedChunks(std::vector<ChunkCords>& neededChunks);

    public:
        /**
         * @brief Constructs the ChunkManager with a specific seed for procedural generation.
         * @param seed Seed used to initialize noise generators.
         */
        ChunkManager(uint seed);

        /**
         * @brief Returns a constant reference to the currently loaded chunks.
         * @return Const reference to the internal chunk map.
         */
        const CordChunkMap& getLoadedChunks() const;

        /**
         * @brief Updates which chunks should be loaded or unloaded based on player position.
         * @param playerPosition The current position of the player.
         * @param renderDistance The distance (in chunks) around the player to keep loaded.
         * @return true if any changes were made (chunks loaded/unloaded), false otherwise.
         */
        bool updateChunks(Vec3 playerPosition, int renderDistance);

        /**
         * @brief Retrieves the block at a specific world-space block position.
         * @param blockPosition 3D integer coordinates of the block.
         * @return The BlockID at the specified position.
         */
        BlockID getBlockAt(iVec3 blockPosition);

        /**
         * @brief Replaces the block at a specific position with a new block ID.
         * @param blockPosition 3D integer coordinates of the block to replace.
         * @param block The new block ID to place at the given position.
         */
        void replaceBlock(iVec3 blockPosition, BlockID block);
};

