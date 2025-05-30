#include "ChunkBuilder.hpp"
#include <iostream>


ChunkBuilder::ChunkBuilder(unsigned int numberOfThreads,
        const Noise& noise) : noise(noise) {
    threads.reserve(numberOfThreads);

    for (unsigned int i = 0; i < numberOfThreads; i++) {
        threads.emplace_back([this] {
                while (true) {
                    ChunkBuildData data;
                    std::unique_lock<std::mutex> lock(buildQueueMutex);  
                    buildQueueCondition.wait(lock, [this] { 
                            return stop || !chunksToGenerate.empty() 
                            || !chunksToBuild.empty(); 
                        });

                    if (stop && chunksToGenerate.empty() 
                            && chunksToBuild.empty()) {
                        return;
                    }
                    
                    if (!chunksToGenerate.empty()) {
                        data = chunksToGenerate.front();
                        chunksToGenerate.pop();
                        lock.unlock();
                        if (data.chunk && data.isNew) {
                            data.chunk->generateTerrain(this->noise);
                            lock.lock();
                            chunksToBuild.emplace(data);
                            lock.unlock();
                            ChunkCords cords = data.chunk->getCords();
                            std::cout << "Generating terrain at: " << cords.x << " " << cords.z << "\n";
                        }
                        continue;
                    } else {

                        data = chunksToBuild.front();
                        chunksToBuild.pop();
                        lock.unlock();

                        if (data.chunk) {
                            data.chunk->buildMesh(data.chunkNeighbors);
                            std::lock_guard<std::mutex> lock(resultMutex);
                            builtChunks.emplace(data.chunk); 
                            builtChunksCondition.notify_one();
                        }
                    }
                }
                });

    }
}

ChunkBuilder::~ChunkBuilder() {
    std::unique_lock<std::mutex> lock(buildQueueMutex);
    stop = true;
    lock.unlock();
    buildQueueCondition.notify_all(); 
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void ChunkBuilder::enqueueChunk(const ChunkBuildData& chunkData) {
    std::unique_lock<std::mutex> lock(buildQueueMutex);
    if (chunkData.isNew) {
        chunksToGenerate.emplace(chunkData);
    } else {
        chunksToBuild.emplace(chunkData);
    }
    neededChunksCounter++;
    lock.unlock();
    buildQueueCondition.notify_one();
}

void ChunkBuilder::pollFinishedChunks() {
    std::unique_lock<std::mutex> lock(resultMutex); 
    builtChunksCondition.wait(lock, [this] {
            return builtChunks.size() == neededChunksCounter;
            }); 
    while (!builtChunks.empty()) {
        Chunk* chunk = builtChunks.front();
        builtChunks.pop();
        lock.unlock();
        chunk->updateMeshData();
        lock.lock();
    }
    neededChunksCounter = 0;
}
