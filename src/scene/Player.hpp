#pragma once 

#include "Camera.hpp"

class Player {
    private:
        eng::PerspectiveCamera& camera;
    public:
        Player(eng::PerspectiveCamera& camera);
        const Vec3 getPosition() const;
        void update();

};
