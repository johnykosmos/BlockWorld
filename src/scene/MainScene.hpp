#pragma once

#include "Engine.hpp"
#include "Material.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "Shapes.hpp"
#include "Texture.hpp"
#include "world/ChunkManager.hpp"

#define RENDER_DISTANCE 8

class MainScene : public eng::Scene {
    private:
        eng::Engine& engine = eng::Engine::getInstance();
        eng::Renderer renderer = engine.getRenderer();
        eng::PerspectiveCamera camera = eng::PerspectiveCamera(Vec3(-10.0f, 10.0f, 0.0f), (float)4/3, 0.0f, 0.0f);
        eng::Shader shader = eng::Shader("assets/shaders/test.shader");

        eng::TextureSpec cobbleSpec = {
            .horizontalWrap = eng::Repeat,
            .verticalWrap = eng::Repeat,
            .minFilter = eng::Nearest,
            .magFilter = eng::Nearest
        };
        eng::Texture cobble = eng::Texture("assets/textures/cobble.jpeg", cobbleSpec);
        eng::Material material = {
            .shader = &shader,
            .texture = &cobble,
            .albedo = Vec3(0.6f, 0.6f, 0.6f)
        };
        
        eng::CubeShape cube;
        ChunkManager chunkMng;

    public:
        MainScene();
        void updateCamera();
        void onUpdate() override;
};
