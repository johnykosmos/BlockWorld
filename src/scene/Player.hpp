#pragma once 

#include "Engine.hpp"
#include "Camera.hpp"
#include "Typedefs.hpp"
#include "world/ChunkManager.hpp"

#define PLAYER_RANGE 5


/**
 * @class Player
 * @brief Represents the player in the game world, handling input, camera updates, and block interactions.
 */
class Player {
private:
    /**
     * @brief Reference to the player's camera.
     */
    eng::PerspectiveCamera& camera;

    /**
     * @brief Input handler reference from the engine singleton.
     */
    eng::Input input = eng::Engine::getInstance().getInputHandler();

    /**
     * @brief Reference to the chunk manager, used for managing world chunks.
     */
    ChunkManager& chunkMng;

    /**
     * @brief Performs a raycast to detect if a block is being targeted.
     * @param hitPosition Output parameter to store the position of the block hit by the ray.
     * @param placePosition Output parameter to store the position to place a new block.
     * @return true if a block was hit, false otherwise.
     */
    bool raycastBlock(iVec3& hitPosition, iVec3& placePosition);

    /**
     * @brief Attempts to place a block in the world at the targeted location.
     */
    void placeBlock();

    /**
     * @brief Attempts to destroy a block in the world at the targeted location.
     */
    void destroyBlock();

    /**
     * @brief Updates the camera based on player input or state.
     */
    void updateCamera();

public:
    /**
     * @brief Constructs a new Player object.
     * @param camera Reference to the camera associated with the player.
     * @param chunkMng Reference to the chunk manager responsible for world data.
     */
    Player(eng::PerspectiveCamera& camera, ChunkManager& chunkMng);

    /**
     * @brief Gets the current position of the player.
     * @return The position of the player as a Vec3.
     */
    const Vec3 getPosition() const;

    /**
     * @brief Updates the player state each frame, including input handling and camera updates.
     */
    void update();
};

