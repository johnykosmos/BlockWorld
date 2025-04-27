#pragma once

#include "Typedefs.hpp"
#include "Entity.hpp"

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 16
#define CHUNK_SIZE_Z 16

#define CUBE_FACES 6
#define VERTICES_PER_FACE 4
#define INDICES_PER_FACE 6

enum class BlockID : int {
    Air = 0,
    Grass = 1,
    Stone = 2
};

struct Face {
    Vec3 position[VERTICES_PER_FACE];
    Vec3 normal;
};

class Chunk : public eng::Entity {
    private:
        BlockID blocks[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];
        iVec2 worldPosition;
        eng::Mesh mesh;

    private: 
        void appendFaceVertices(const Vec3& coordinates, const Face& face, 
                std::vector<eng::Vertex>& vertices,
                std::vector<unsigned int>& indices);

    public:
        const eng::Mesh& getMesh() const override;
        BlockID getBlock(int x, int y, int z) const;
        void setBlock(int x, int y, int z, BlockID blockID);
        void buildMesh();
};
