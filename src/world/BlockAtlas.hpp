#pragma once

#include "Typedefs.hpp"
#include <map>


enum class BlockID : char {
    Air = 0,
    Dirt = 1,
    Grass = 2,
    Stone = 3,
    Sand = 4,
    Log = 5,
    Leaves = 6,
    Water = 7
};

struct BlockData {
    const Vec2 topUV;
    const Vec2 bottomUV;
    const Vec2 frontUV;
    const Vec2 backUV;
    const Vec2 leftUV;
    const Vec2 rightUV;
};

class BlockAtlas {
    private:
        static std::map<BlockID, BlockData> blockMap; 

    private:
        static bool doesBlockExist(BlockID block);

    public:
        static const Vec2 getFaceTextureUV(BlockID block, Vec3 faceNormal);

};
