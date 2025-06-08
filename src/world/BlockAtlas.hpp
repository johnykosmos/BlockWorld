#pragma once

#include "Typedefs.hpp"
#include <map>



/**
 * @enum BlockID
 * @brief Enumerates different types of blocks in the game world.
 */
enum class BlockID : char {
    Air = 0,    /**< Represents empty space or no block */
    Dirt = 1,   /**< Dirt block */
    Grass = 2,  /**< Grass-covered dirt block */
    Stone = 3,  /**< Stone block */
    Sand = 4,   /**< Sand block */
    Log = 5,    /**< Tree log block */
    Leaves = 6, /**< Leaves block */
    Water = 7   /**< Water block */
};

/**
 * @struct BlockData
 * @brief Stores texture UV coordinates for each face of a block.
 */
struct BlockData {
    const Vec2 topUV;     /**< UV coordinates for the top face */
    const Vec2 bottomUV;  /**< UV coordinates for the bottom face */
    const Vec2 frontUV;   /**< UV coordinates for the front face */
    const Vec2 backUV;    /**< UV coordinates for the back face */
    const Vec2 leftUV;    /**< UV coordinates for the left face */
    const Vec2 rightUV;   /**< UV coordinates for the right face */
};

/**
 * @class BlockAtlas
 * @brief Provides mapping from BlockID and face orientation to texture UV coordinates.
 */
class BlockAtlas {
    private:
        /**
         * @brief Maps each BlockID to its corresponding BlockData.
         */
        static std::map<BlockID, BlockData> blockMap;

        /**
         * @brief Checks if a block type exists in the block map.
         * @param block The BlockID to check.
         * @return true if the block exists, false otherwise.
         */
        static bool doesBlockExist(BlockID block);

    public:
        /**
         * @brief Retrieves the texture UV coordinates for a specific block face.
         * @param block The BlockID of the block.
         * @param faceNormal The normal vector of the face to get UVs for.
         * @return UV coordinates corresponding to the block face.
         */
        static const Vec2 getFaceTextureUV(BlockID block, Vec3 faceNormal);
};

