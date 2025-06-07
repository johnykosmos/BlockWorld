#include "Chunk.hpp"
#include "utils/LCG.hpp"
#include "world/BlockAtlas.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>

Face cubeFaces[6] = {
    // Right
    {
        .position = { {1,0,0}, {1,1,0}, {1,1,1}, {1,0,1} },
        .normal = {1,0,0}
    },
    // Left
    {
        .position = { {0,0,1}, {0,1,1}, {0,1,0}, {0,0,0} },
        .normal = {-1,0,0}
    },
    // Front
    {
        .position = { {1,0,1}, {1,1,1}, {0,1,1}, {0,0,1} },
        .normal = {0,0,1}
    },
    // Back
    {
        .position = { {0,0,0}, {0,1,0}, {1,1,0}, {1,0,0} },
        .normal = {0,0,-1}
    },
    // Top
    {
        .position = { {0,1,0}, {0,1,1}, {1,1,1}, {1,1,0} },
        .normal = {0,1,0}
    },
    // Bottom
    {
        .position = { {0,0,1}, {0,0,0}, {1,0,0}, {1,0,1} },
        .normal = {0,-1,0}
    }
};

uint cubeFaceIndices[INDICES_PER_FACE] = {
    0, 1, 2,
    0, 2, 3
};

Vec2 textureUV[4] = {
    {0.0f, BASE_UV},
    {0.0f, 0.0f}, 
    {BASE_UV, 0.0f},     
    {BASE_UV, BASE_UV}, 
};

bool ChunkCords::operator==(const ChunkCords& other) const {
    return (this->x == other.x && this->z == other.z) ? true : false;
}

bool ChunkCords::operator<(const ChunkCords& other) const {
        if (x != other.x) {
            return x < other.x; 
        }
        return z < other.z; 
    }

ChunkCords ChunkCords::operator+(const ChunkCords& other) const {
    return ChunkCords{this->x + other.x, this->z + other.z};
}

Chunk::Chunk(ChunkCords position) : position(position) {
    memset(blocks, 0, sizeof(blocks));
    this->translate(Vec3{position.x * CHUNK_SIZE_X, 0.0f, position.z * CHUNK_SIZE_Z});
}

const eng::Mesh& Chunk::getMesh() const {
    return mesh;
}


const ChunkCords Chunk::getCords() const {
    return position;
}

BlockID Chunk::getBlock(int x, int y, int z) const {
    return blocks[x][y][z];
}

void Chunk::setBlock(int x, int y, int z, BlockID blockID) {
    blocks[x][y][z] = blockID;
}

void Chunk::setDirty() {
    dirty = true;
}

void Chunk::fillWater() {
    for (int x = 0; x < CHUNK_SIZE_X; x++) {
        for (int z = 0; z < CHUNK_SIZE_Z; z++) {
            if (getBlock(x, SEA_TRESHOLD, z) == BlockID::Air) {
                setBlock(x, SEA_TRESHOLD, z, BlockID::Water);
            }
        }
    }
}

void Chunk::generateTerrain(const Noise& noise) {
    for (int x = 0; x < CHUNK_SIZE_X; x++) {
        for (int z = 0; z < CHUNK_SIZE_Z; z++) {
            float worldX = float(position.x * CHUNK_SIZE_X + x);
            float worldZ = float(position.z * CHUNK_SIZE_Z + z);
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
                setBlock(x, y, z, block);
            }
        }
    }
    fillWater();
}

void Chunk::placeTree(iVec3 position, uint chunkSeed, 
        Chunk* neighbors[]) {  
    LCG lcg(chunkSeed + 222);
    uint height = static_cast<uint>(lcg.nextFloatInRange(
                                TREE_HEIGHT_MIN, TREE_HEIGHT_MAX));
    for (uint y = position.y; y <= position.y + height; y++) {
        if (y >= CHUNK_SIZE_Y) break;
        setBlock(position.x, y, position.z, BlockID::Log); 
    }

    int currentLayer = 0;
    int layerRange = 0;
    for (uint y = height; y < height + 4; y++) {
        currentLayer++;
        if (currentLayer % 3 == 0) layerRange++;
        for (int x = -2 + layerRange; x <= 2 - layerRange; x++) {
            for (int z = -2 + layerRange; z <= 2 - layerRange; z++) {
                int nX = position.x + x;
                int nY = position.y + y;
                int nZ = position.z + z;
                if (nX >= 0 && nX < CHUNK_SIZE_X && nY >= 0 && 
                        nY < CHUNK_SIZE_Y && nZ >= 0 && nZ < CHUNK_SIZE_Z) {
                    if (getBlock(nX, nY, nZ) == BlockID::Air) {
                        setBlock(nX, nY, nZ, BlockID::Leaves);
                    }
                } else {
                    int neighborX = (nX < 0) ? -1 : (nX >= CHUNK_SIZE_X ? 1 : 0);
                    int neighborZ = (nZ < 0) ? -1 : (nZ >= CHUNK_SIZE_Z ? 1 : 0);

                    ChunkCords neighborCords = {
                        .x = this->position.x + neighborX,
                        .z = this->position.z + neighborZ
                    };
                    for (int i = 0; i < 4; i++) {
                        if (neighbors[i] && 
                                neighbors[i]->getCords() == neighborCords) {
                            nX = (nX + CHUNK_SIZE_X) % CHUNK_SIZE_X;
                            nZ = (nZ + CHUNK_SIZE_Z) % CHUNK_SIZE_Z; 
                            if (neighbors[i]->getBlock(nX, nY, nZ) == 
                                    BlockID::Air) {
                                neighbors[i]->setBlock(nX, nY, nZ, 
                                        BlockID::Leaves);
                            }
                        }
                    }
                }
            }
        }
    }
}

void Chunk::decorateTerrain(uint worldSeed, Chunk* neighbors[]) {
    uint chunkSeed = worldSeed + position.x * position.z;
    LCG lcg(chunkSeed);
    for (int x = 0; x < CHUNK_SIZE_X; x++) {
        for (int z = 0; z < CHUNK_SIZE_Z; z++) { 
            int y;
            for (y = CHUNK_SIZE_Y - 1; y >= 0; y--) {
                if (getBlock(x, y, z) != BlockID::Air) break;
            }

            if (getBlock(x, y, z) == BlockID::Grass) {
                if (lcg.nextFloat() < 0.002f) {
                    placeTree(iVec3{x, y + 1, z}, chunkSeed, neighbors);
                }
            }
        }
    }
    std::cout << "Decorating chunk at: " << position.x << " " << position.z << "\n";
}

bool Chunk::isNeighborBlockTransparent(const Chunk* neighbor,
        const iVec3 neighborBlockPos) const {
    if (!neighbor) return false;
    return (neighbor->getBlock(neighborBlockPos.x,
                neighborBlockPos.y, neighborBlockPos.z) == BlockID::Air);
}

void Chunk::appendFaceVertices(BlockID block,
        const Vec3& coordinates, const Face& face, 
        std::vector<eng::Vertex>& vertices,
        std::vector<uint>& indices) {
    uint startIndex = vertices.size();
    const Vec2 offsetUV = BlockAtlas::getFaceTextureUV(block, face.normal);
    for (int i = 0; i < VERTICES_PER_FACE; i++) {
        eng::Vertex vertex = {
            .vertices = face.position[i] + coordinates,
            .normal = face.normal,
            .texturePos = Vec2{
                textureUV[i].x + BASE_UV * offsetUV.x,
                textureUV[i].y + BASE_UV * offsetUV.y
            }
        };
        vertices.push_back(vertex);
    }
    
    for (int i = 0; i < INDICES_PER_FACE; i++) {
        indices.push_back(startIndex + cubeFaceIndices[i]);
    }
}

bool Chunk::isDirty() const {
    return dirty;
}

void Chunk::buildMesh(Chunk* neighbors[]) {
    vertices.clear();
    indices.clear();
    for (int x = 0; x < CHUNK_SIZE_X; x++) {
        for (int y = 0; y < CHUNK_SIZE_Y; y++) {
            for (int z = 0; z < CHUNK_SIZE_Z; z++) {
                BlockID block = blocks[x][y][z]; 
                if (block == BlockID::Air) 
                    continue;

                for (int dir = 0; dir < CUBE_FACES; dir++) {
                    Face face = cubeFaces[dir]; 
                    int nX = x + (int)face.normal.x;
                    int nY = y + (int)face.normal.y;
                    int nZ = z + (int)face.normal.z;
                    if (nX >= 0 && nX < CHUNK_SIZE_X && 
                            nY >= 0 && nY < CHUNK_SIZE_Y && 
                            nZ >= 0 && nZ < CHUNK_SIZE_Z) {
                        if (blocks[nX][nY][nZ] == BlockID::Air) {
                            appendFaceVertices(block, Vec3{x, y, z}, 
                                    face, vertices, indices);
                        }
                    } else if (dir < 4) {
                        int neighborX = (nX + CHUNK_SIZE_X) % CHUNK_SIZE_X;
                        int neighborZ = (nZ + CHUNK_SIZE_Z) % CHUNK_SIZE_Z;
                        if(isNeighborBlockTransparent(neighbors[dir], iVec3{neighborX, y, neighborZ})) {
                            appendFaceVertices(block, Vec3{x, y, z}, 
                                    face, vertices, indices);               
                        }
                    }
                }

            }
        }
    }
    
    dirty = false; 
}

void Chunk::updateMeshData() {
    mesh.updateData(vertices.data(), vertices.size(), indices.data(), indices.size());
}
