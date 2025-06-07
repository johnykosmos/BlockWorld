#pragma once


#include "world/Chunk.hpp"
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

struct ChunkBuildData {
    Chunk* chunk;
    Chunk* chunkNeighbors[4];
    bool isNew;
};

class ChunkBuilder {
    private:
        const Noise noise;
        const uint worldSeed;
        std::vector<std::thread> threads;
        std::queue<ChunkBuildData> chunksToGenerate;
        std::queue<ChunkBuildData> chunksToDecorate;
        std::queue<ChunkBuildData> chunksToBuild;
        std::queue<Chunk*> builtChunks;
        std::mutex buildQueueMutex;
        std::mutex resultMutex;
        std::condition_variable buildQueueCondition;
        std::condition_variable builtChunksCondition;
        bool stop = false;
        uint neededChunksCounter = 0;

    public:
        ChunkBuilder(uint numberOfThreads, uint worldSeed, const Noise& noise);
        ~ChunkBuilder();
        void enqueueChunk(const ChunkBuildData& chunkData);
        void pollFinishedChunks();
};
