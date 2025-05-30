#include "BlockAtlas.hpp"

std::map<BlockID, BlockData> BlockAtlas::blockMap = {
    {
        BlockID::Dirt, 
        {
            .topUV = Vec2{0, 0}, 
            .bottomUV = Vec2{0, 0},
            .frontUV = Vec2{0, 0},
            .backUV = Vec2{0, 0},
            .leftUV = Vec2{0, 0},
            .rightUV = Vec2{0, 0},
        }
    },
    {
        BlockID::Grass, 
        {
            .topUV = Vec2{2, 0}, 
            .bottomUV = Vec2{0, 0},
            .frontUV = Vec2{1, 0},
            .backUV = Vec2{1, 0},
            .leftUV = Vec2{1, 0},
            .rightUV = Vec2{1, 0},
        }
    },
    {
        BlockID::Stone, 
        {
            .topUV = Vec2{1, 1}, 
            .bottomUV = Vec2{1, 1},
            .frontUV = Vec2{1, 1},
            .backUV = Vec2{1, 1},
            .leftUV = Vec2{1, 1},
            .rightUV = Vec2{1, 1},
        }
    },
    {
        BlockID::Sand, 
        {
            .topUV = Vec2{0, 1}, 
            .bottomUV = Vec2{0, 1},
            .frontUV = Vec2{0, 1},
            .backUV = Vec2{0, 1},
            .leftUV = Vec2{0, 1},
            .rightUV = Vec2{0, 1},
        }
    },
    {
        BlockID::Water, 
        {
            .topUV = Vec2{0, 2}, 
            .bottomUV = Vec2{0, 2},
            .frontUV = Vec2{0, 2},
            .backUV = Vec2{0, 2},
            .leftUV = Vec2{0, 2},
            .rightUV = Vec2{0, 2},
        }
    }

};

bool BlockAtlas::doesBlockExist(BlockID block) {
    auto it = blockMap.find(block);
    return (it != blockMap.end());
}

const Vec2 BlockAtlas::getFaceTextureUV(BlockID block, Vec3 faceNormal) {
    const Vec2 nullUV = {0,0};
    if (doesBlockExist(block)) {
        auto it = blockMap.find(block);
        const BlockData blockData = it->second;
        
        if (faceNormal.x == 1.0f) {
            return blockData.rightUV;
        } 
        if (faceNormal.x == -1.0f) {
            return blockData.leftUV;
        } 
        if (faceNormal.y == 1.0f) {
            return blockData.topUV;
        }
        if (faceNormal.y == -1.0f) {
            return blockData.bottomUV;
        }
        if (faceNormal.z == 1.0f) {
            return blockData.frontUV;
        }
        if (faceNormal.z == -1.0f) {
            return blockData.backUV;
        }
    }
    return nullUV;
}

