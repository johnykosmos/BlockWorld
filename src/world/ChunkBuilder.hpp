#pragma once


#include "world/Chunk.hpp"
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

struct ChunkBuildData {
    Chunk* chunk;
    const Chunk* chunkNeighbors[4];
};

class ChunkBuilder {
    private:
        std::vector<std::thread> threads;
        std::queue<ChunkBuildData> chunksToBuild;
        std::queue<Chunk*> builtChunks;
        std::mutex buildQueueMutex;
        std::mutex resultMutex;
        std::condition_variable buildQueueCondition;
        std::condition_variable builtChunksCondition;
        bool stop = false;
        unsigned int neededChunksCounter = 0;

    public:
        ChunkBuilder(unsigned int numberOfThreads);
        ~ChunkBuilder();
        void enqueueChunk(const ChunkBuildData& chunkData);
        void pollFinishedChunks();
};
