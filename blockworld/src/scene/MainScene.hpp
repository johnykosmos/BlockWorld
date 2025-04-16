#pragma once

#include "Scene.hpp"
#include "Camera.hpp"


class MainScene : public eng::Scene {
    private:
        eng::PerspectiveCamera camera = eng::PerspectiveCamera(Vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.0f);

    public:
        MainScene();
        void onUpdate() override;
};
