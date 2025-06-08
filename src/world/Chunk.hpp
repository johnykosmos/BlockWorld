#pragma once

#include "Typedefs.hpp"
#include "Entity.hpp"
#include "BlockAtlas.hpp"
#include "FastNoiseLite.h"

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 64
#define CHUNK_SIZE_Z 16

#define SEA_TRESHOLD 7
#define TREE_HEIGHT_MIN 2.0f
#define TREE_HEIGHT_MAX 6.0f

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
    ChunkCords operator+(const ChunkCords& other) const;
};

class Chunk : public eng::Entity {
    private:
        BlockID blocks[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];
        ChunkCords position;
        eng::Mesh mesh;
        std::vector<eng::Vertex> vertices;
        std::vector<uint> indices;

        bool dirty = true;

    private: 
        bool isNeighborBlockTransparent(const Chunk* neighbor, 
                const iVec3 neighborBlockPos) const;
        void appendFaceVertices(BlockID block, 
                const Vec3& coordinates, const Face& face, 
                std::vector<eng::Vertex>& vertices,
                std::vector<uint>& indices);

    public:
        Chunk(ChunkCords position);
        const eng::Mesh& getMesh() const override;
        const ChunkCords getCords() const;
        BlockID getBlock(int x, int y, int z) const;
        void setBlock(int x, int y, int z, BlockID blockID);
        bool isDirty() const;
        void setDirty();
        void buildMesh(Chunk* neighbors[]);
        void updateMeshData();
};
