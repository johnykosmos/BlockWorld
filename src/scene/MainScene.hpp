#pragma once

#include "Engine.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "Texture.hpp"
#include "Player.hpp"
#include "world/ChunkManager.hpp"

#define RENDER_DISTANCE 12


/**
 * @class MainScene
 * @brief Represents the main scene of the game, handling rendering, player input, and world management.
 * 
 * Inherits from eng::Scene and manages core components such as the camera, renderer, lighting, and chunk data.
 */
class MainScene : public eng::Scene {
    private:
        /**
         * @brief Reference to the engine singleton instance.
         */
        eng::Engine& engine = eng::Engine::getInstance();

        /**
         * @brief Renderer used for drawing the scene.
         */
        eng::Renderer renderer = engine.getRenderer();

        /**
         * @brief Perspective camera used to render the 3D scene.
         */
        eng::PerspectiveCamera camera = eng::PerspectiveCamera(Vec3(-10.0f, 23.0f, 0.0f), (float)4/3, 0.0f, 0.0f);

        /**
         * @brief Shader used for rendering objects in the scene.
         */
        eng::Shader shader = eng::Shader("assets/shaders/test.shader");

        /**
         * @brief Texture specification for the blocks texture.
         */
        eng::TextureSpec blocksSpec = {
            .horizontalWrap = eng::Repeat,
            .verticalWrap = eng::Repeat,
            .minFilter = eng::Nearest,
            .magFilter = eng::Nearest
        };

        /**
         * @brief Texture used for rendering blocks.
         */
        eng::Texture blocks = eng::Texture("assets/textures/blocks.png", blocksSpec);

        /**
         * @brief Material defining surface properties for rendered objects.
         */
        eng::Material material = {
            .shader = &shader,
            .texture = &blocks,
            .albedo = Vec3(0.6f, 0.6f, 0.6f),
            .ambient = Vec3(0.5f, 0.5f, 0.5f),
            .diffuse = Vec3(0.6f, 0.6f, 0.6f),
            .specular = Vec3(0.0f, 0.0f, 0.0f),
            .shininess = 32.0f
        };

        /**
         * @brief Base directional light used for the scene.
         */
        eng::DirectionalLight baseLight = {
            .color = Vec3(1, 1, 1),
            .direction = Vec3(0, -1, 1)
        };

        /**
         * @brief Manages world chunks and procedural terrain.
         */
        ChunkManager chunkMng;

        /**
         * @brief The player object representing the user in the scene.
         */
        Player player;

    public:
        /**
         * @brief Constructs the MainScene and initializes scene components.
         */
        MainScene();

        /**
         * @brief Updates the scene each frame.
         * 
         * Handles input, player movement, and rendering updates.
         */
        void onUpdate() override;
};

