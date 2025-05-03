#include "Chunk.hpp"
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

unsigned int cubeFaceIndices[INDICES_PER_FACE] = {
    0, 1, 2,
    0, 2, 3
};

Vec2 faceUVs[4] = {
        {0.0f, 0.0f}, 
        {0.0f, 1.0f}, 
        {1.0f, 1.0f}, 
        {1.0f, 0.0f}  
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

Chunk::Chunk(ChunkCords position) : position(position) {
    for (int x = 0; x < CHUNK_SIZE_X; x++) {
        for (int y = 0; y < CHUNK_SIZE_Y; y++) {
            for (int z = 0; z < CHUNK_SIZE_Z; z++) {
                if (y > 6)
                    setBlock(x, y, z, BlockID::Air);       
                else
                    setBlock(x, y, z, BlockID::Stone);
            }
        }
    }
    this->translate(Vec3{position.x * CHUNK_SIZE_X, 0.0f, position.z * CHUNK_SIZE_Z});
}

const eng::Mesh& Chunk::getMesh() const {
    return mesh;
}

BlockID Chunk::getBlock(int x, int y, int z) const {
    return blocks[x][y][z];
}

void Chunk::setBlock(int x, int y, int z, BlockID blockID) {
    blocks[x][y][z] = blockID;
}

bool Chunk::isNeighborBlockTransparent(const Chunk& neighbor,
        const iVec3 neighborBlockPos) const {
    return (neighbor.getBlock(neighborBlockPos.x,
                neighborBlockPos.y, neighborBlockPos.z) == BlockID::Air);
}

void Chunk::appendFaceVertices(const Vec3& coordinates, const Face& face, 
        std::vector<eng::Vertex>& vertices,
        std::vector<unsigned int>& indices) {
    unsigned int startIndex = vertices.size();
    for (int i = 0; i < VERTICES_PER_FACE; i++) {
        eng::Vertex vertex = {
            .vertices = face.position[i] + coordinates,
            .normal = face.normal,
            .texturePos = faceUVs[i] 
        };

        vertices.push_back(vertex);
    }
    
    for (int i = 0; i < INDICES_PER_FACE; i++) {
        indices.push_back(startIndex + cubeFaceIndices[i]);
    }
}

void Chunk::buildMesh(const Chunk* neighbors[]) {
    std::vector<eng::Vertex> vertices;
    std::vector<unsigned int> indices;
    for (int x = 0; x < CHUNK_SIZE_X; x++) {
        for (int y = 0; y < CHUNK_SIZE_Y; y++) {
            for (int z = 0; z < CHUNK_SIZE_Z; z++) {
                if (blocks[x][y][z] == BlockID::Air) 
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
                            appendFaceVertices(Vec3{x, y, z}, 
                                    face, vertices, indices);
                        }
                    }else if (neighbors[dir]) {
                        int neighborX = (nX + CHUNK_SIZE_X) % CHUNK_SIZE_X;
                        int neighborZ = (nZ + CHUNK_SIZE_Z) % CHUNK_SIZE_Z;
                        if(isNeighborBlockTransparent(*neighbors[dir], iVec3{neighborX, y, neighborZ})) {
                            assert(dir <= 3);
                            appendFaceVertices(Vec3{x, y, z}, 
                                    face, vertices, indices);               
                        }
                    }
                }

            }
        }
    }
    
    mesh.updateData(vertices.data(), vertices.size(), indices.data(), indices.size());
    
}
