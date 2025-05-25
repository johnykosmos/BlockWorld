#pragma once

#include "Typedefs.hpp"
#include "Entity.hpp"
#include "BlockAtlas.hpp"
#include "FastNoiseLite.h"

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 32
#define CHUNK_SIZE_Z 16

#define SEA_TRESHOLD 13

#define CUBE_FACES 6
#define VERTICES_PER_FACE 4
#define INDICES_PER_FACE 6

constexpr float BASE_UV = 1.0f/4;

struct Face {
    Vec3 position[VERTICES_PER_FACE];
    Vec3 normal;
};

struct ChunkCords {
    int x, z;

    bool operator==(const ChunkCords& other) const;
    bool operator<(const ChunkCords& other) const;
};

struct Noise {
    const FastNoiseLite* base;
    const FastNoiseLite* detail;
};

class Chunk : public eng::Entity {
    private:
        BlockID blocks[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];
        ChunkCords position;
        eng::Mesh mesh;
        std::vector<eng::Vertex> vertices;
        std::vector<unsigned int> indices;

        bool dirty = true;

    private: 
        bool isNeighborBlockTransparent(const Chunk* neighbor, 
                const iVec3 neighborBlockPos) const;
        void appendFaceVertices(BlockID block, 
                const Vec3& coordinates, const Face& face, 
                std::vector<eng::Vertex>& vertices,
                std::vector<unsigned int>& indices);

    public:
        Chunk(ChunkCords position);
        const eng::Mesh& getMesh() const override;
        const ChunkCords getCords() const;
        BlockID getBlock(int x, int y, int z) const;
        void setBlock(int x, int y, int z, BlockID blockID);
        bool isDirty() const;
        void generateTerrain(const Noise& noise);
        void buildMesh(const Chunk* neighbors[]);
        void updateMeshData();
};
