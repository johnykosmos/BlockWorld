#pragma once

#include "Entity.hpp"
#include "BlockAtlas.hpp"
#include "utils/CubeRenderParameters.hpp"

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 64
#define CHUNK_SIZE_Z 16

#define SEA_TRESHOLD 7
#define TREE_HEIGHT_MIN 2.0f
#define TREE_HEIGHT_MAX 6.0f


/**
 * @struct ChunkCords
 * @brief Represents 2D chunk coordinates in the world.
 */
struct ChunkCords {
    int x; /**< X coordinate of the chunk */
    int z; /**< Z coordinate of the chunk */

    /**
     * @brief Equality operator to compare two ChunkCords.
     * @param other The other ChunkCords to compare.
     * @return true if both x and z coordinates are equal.
     */
    bool operator==(const ChunkCords& other) const;

    /**
     * @brief Less-than operator for ordering ChunkCords (useful for std::map or sets).
     * @param other The other ChunkCords to compare.
     * @return true if this chunk is less than the other based on x and z.
     */
    bool operator<(const ChunkCords& other) const;

    /**
     * @brief Addition operator to add two ChunkCords.
     * @param other The other ChunkCords to add.
     * @return A new ChunkCords representing the sum.
     */
    ChunkCords operator+(const ChunkCords& other) const;
};

/**
 * @class Chunk
 * @brief Represents a 3D chunk of blocks in the world, manages blocks and mesh data.
 * 
 * Inherits from eng::Entity for integration with the engine's entity system.
 */
class Chunk : public eng::Entity {
    private:
        /**
         * @brief 3D array storing block IDs for the chunk.
         */
        BlockID blocks[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];

        /**
         * @brief Coordinates of the chunk in the world.
         */
        ChunkCords position;

        /**
         * @brief Mesh used to render the chunk.
         */
        eng::Mesh mesh;

        /**
         * @brief Vertices for the chunk's mesh.
         */
        std::vector<eng::Vertex> vertices;

        /**
         * @brief Indices for the chunk's mesh.
         */
        std::vector<uint> indices;

        /**
         * @brief Flag indicating whether the chunk's mesh data needs to be rebuilt.
         */
        bool dirty = true;

    private:
        /**
         * @brief Checks if a neighboring block is transparent for face culling.
         * @param neighbor Pointer to the neighboring chunk.
         * @param neighborBlockPos Position of the block inside the neighbor chunk.
         * @return true if the neighbor block is transparent, false otherwise.
         */
        bool isNeighborBlockTransparent(const Chunk* neighbor, const iVec3 neighborBlockPos) const;

        /**
         * @brief Appends vertices and indices for a block face to the mesh data.
         * @param block The block ID of the face being appended.
         * @param coordinates World coordinates of the block.
         * @param face The specific face of the block to append.
         * @param vertices Vector to append vertex data.
         * @param indices Vector to append index data.
         */
        void appendFaceVertices(BlockID block, const Vec3& coordinates, const Face& face,
                std::vector<eng::Vertex>& vertices, std::vector<uint>& indices);

    public:
        /**
         * @brief Constructs a Chunk at the given chunk coordinates.
         * @param position Coordinates identifying the chunk position.
         */
        Chunk(ChunkCords position);

        /**
         * @brief Gets the mesh representing the chunk.
         * @return Const reference to the chunk's mesh.
         */
        const eng::Mesh& getMesh() const override;

        /**
         * @brief Gets the coordinates of the chunk.
         * @return The chunk's coordinates.
         */
        const ChunkCords getCords() const;

        /**
         * @brief Retrieves the block ID at specified local coordinates.
         * @param x Local X coordinate within the chunk.
         * @param y Local Y coordinate within the chunk.
         * @param z Local Z coordinate within the chunk.
         * @return BlockID at the given coordinates.
         */
        BlockID getBlock(int x, int y, int z) const;

        /**
         * @brief Sets the block ID at specified local coordinates.
         * @param x Local X coordinate within the chunk.
         * @param y Local Y coordinate within the chunk.
         * @param z Local Z coordinate within the chunk.
         * @param blockID The block ID to set.
         */
        void setBlock(int x, int y, int z, BlockID blockID);

        /**
         * @brief Checks whether the chunk mesh needs to be rebuilt.
         * @return true if the chunk is dirty, false otherwise.
         */
        bool isDirty() const;

        /**
         * @brief Marks the chunk mesh as needing to be rebuilt.
         */
        void setDirty();

        /**
         * @brief Builds the mesh for the chunk using information about neighbor chunks.
         * @param neighbors Array of pointers to the four neighboring chunks (N, E, S, W).
         */
        void buildMesh(Chunk* neighbors[]);

        /**
         * @brief Updates the mesh data (vertices and indices) after building.
         */
        void updateMeshData();

        /**
         * @brief Converts world block coordinates to chunk coordinates.
         * @param worldPos The 3D world position of a block.
         * @return ChunkCords corresponding to the chunk containing the block.
         */
        static ChunkCords worldToChunk(iVec3 worldPos);
};

