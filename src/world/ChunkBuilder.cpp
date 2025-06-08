#include "ChunkBuilder.hpp"
#include "world/TerrainGenerator.hpp"
#include <iostream>


ChunkBuilder::ChunkBuilder(uint numberOfThreads, uint worldSeed,
        const Noise& noise) : worldSeed(worldSeed), noise(noise) {
    threads.reserve(numberOfThreads);

    for (uint i = 0; i < numberOfThreads; i++) {
        threads.emplace_back([this] {
                while (true) {
                    ChunkBuildData data;
                    std::unique_lock<std::mutex> lock(buildQueueMutex);  
                    buildQueueCondition.wait(lock, [this] { 
                            return stop || !chunksToGenerate.empty()
                            || !chunksToDecorate.empty()
                            || !chunksToBuild.empty(); 
                        });

                    if (stop && chunksToGenerate.empty() 
                            && chunksToBuild.empty() && 
                            chunksToDecorate.empty()) {
                        return;
                    }
                    
                    if (!chunksToGenerate.empty()) {
                        data = chunksToGenerate.front();
                        chunksToGenerate.pop();
                        lock.unlock();
                        if (data.chunk && data.isNew) {
                            TerrainGenerator::generateTerrain(data.chunk, 
                                    this->noise);
                            lock.lock();
                            chunksToDecorate.emplace(data);
                            lock.unlock();
                            ChunkCords cords = data.chunk->getCords();
                            std::cout << "Generating terrain at: " << cords.x << " " << cords.z << "\n";
                        }
                        continue;
                    } else if (!chunksToDecorate.empty()) {
                        data = chunksToDecorate.front();
                        chunksToDecorate.pop();
                        lock.unlock();
                        if (data.chunk && data.isNew) {
                            TerrainGenerator::decorateTerrain(data, 
                                    this->worldSeed);
                            lock.lock();
                            chunksToBuild.emplace(data);
                            lock.unlock();
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
