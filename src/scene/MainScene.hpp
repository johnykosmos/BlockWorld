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

class MainScene : public eng::Scene {
    private:
        eng::Engine& engine = eng::Engine::getInstance();
        eng::Renderer renderer = engine.getRenderer();
        eng::PerspectiveCamera camera = eng::PerspectiveCamera(Vec3(-10.0f, 23.0f, 0.0f), (float)4/3, 0.0f, 0.0f);
        eng::Shader shader = eng::Shader("assets/shaders/test.shader");

        eng::TextureSpec blocksSpec = {
            .horizontalWrap = eng::Repeat,
            .verticalWrap = eng::Repeat,
            .minFilter = eng::Nearest,
            .magFilter = eng::Nearest
        };
        eng::Texture blocks = eng::Texture("assets/textures/blocks.png", blocksSpec);
        eng::Material material = {
            .shader = &shader,
            .texture = &blocks,
            .albedo = Vec3(0.6f, 0.6f, 0.6f),
            .ambient = Vec3(0.5f, 0.5f, 0.5f),
            .diffuse = Vec3(0.6f, 0.6f, 0.6f),
            .specular = Vec3(0.0f, 0.0f, 0.0f),
            .shininess = 32.0f
        };
        eng::DirectionalLight baseLight = {
            .color = Vec3(1,1,1),
            .direction = Vec3(0,-1,1)
        }; 
        ChunkManager chunkMng = ChunkManager(1337);
        Player player = Player(camera, chunkMng);

    public:
        MainScene();
        void onUpdate() override;
};
