#pragma once

#include "world/Chunk.hpp"
#include "TerrainGenerator.hpp"
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

/**
 * @class ChunkBuilder
 * @brief Handles multithreaded chunk generation, decoration, and mesh building.
 * 
 * Manages task queues and worker threads to efficiently process chunk data asynchronously.
 */
class ChunkBuilder {
    private:
        /**
         * @brief Noise generators used for terrain generation.
         */
        const Noise noise;

        /**
         * @brief World seed used for deterministic generation.
         */
        const uint worldSeed;

        /**
         * @brief Worker threads for concurrent chunk processing.
         */
        std::vector<std::thread> threads;

        /**
         * @brief Queue of chunks to be generated.
         */
        std::queue<ChunkBuildData> chunksToGenerate;

        /**
         * @brief Queue of chunks to be decorated (e.g., trees, features).
         */
        std::queue<ChunkBuildData> chunksToDecorate;

        /**
         * @brief Queue of chunks to be built into renderable mesh data.
         */
        std::queue<ChunkBuildData> chunksToBuild;

        /**
         * @brief Queue of chunks that have completed building and are ready to use.
         */
        std::queue<Chunk*> builtChunks;

        /**
         * @brief Mutex for synchronizing access to the build queues.
         */
        std::mutex buildQueueMutex;

        /**
         * @brief Mutex for synchronizing access to the completed chunks.
         */
        std::mutex resultMutex;

        /**
         * @brief Condition variable for waking worker threads when new build tasks are available.
         */
        std::condition_variable buildQueueCondition;

        /**
         * @brief Condition variable for signaling availability of completed chunks.
         */
        std::condition_variable builtChunksCondition;

        /**
         * @brief Flag to signal threads to stop processing.
         */
        bool stop = false;

        /**
         * @brief Counter for tracking how many chunks are needed for processing.
         */
        uint neededChunksCounter = 0;

    public:
        /**
         * @brief Constructs the ChunkBuilder with a specified number of worker threads.
         * @param numberOfThreads Number of threads to use for processing.
         * @param worldSeed Seed used for deterministic generation.
         * @param noise Reference to the noise configuration used for terrain generation.
         */
        ChunkBuilder(uint numberOfThreads, uint worldSeed, const Noise& noise);

        /**
         * @brief Destructor that ensures worker threads are properly terminated.
         */
        ~ChunkBuilder();

        /**
         * @brief Adds a chunk to the generation queue.
         * @param chunkData Data structure containing the chunk and its context.
         */
        void enqueueChunk(const ChunkBuildData& chunkData);

        /**
         * @brief Polls for completed chunks and processes them accordingly.
         * 
         * This is typically called from the main thread to collect built chunks.
         */
        void pollFinishedChunks();
};

