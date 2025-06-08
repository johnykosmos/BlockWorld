#include "Chunk.hpp"
#include "world/BlockAtlas.hpp"
#include <cstring>

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

ChunkCords Chunk::worldToChunk(iVec3 worldPos) {
    ChunkCords chunkCords = {
        .x = (worldPos.x >= 0) ? worldPos.x / CHUNK_SIZE_X : 
            (worldPos.x - CHUNK_SIZE_X + 1) / CHUNK_SIZE_X ,
        .z = (worldPos.z >= 0) ? worldPos.z / CHUNK_SIZE_Z : 
            (worldPos.z - CHUNK_SIZE_Z + 1) / CHUNK_SIZE_Z
    };
    return chunkCords;
}
