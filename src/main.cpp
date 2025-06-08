#include "Application.hpp"
#include "scene/MainScene.hpp"


class BlockWorld : public eng::Application {
    private:
        MainScene scene;
        
    public:
        explicit BlockWorld() : eng::Application() {
            setScene(scene);
        }

        void onUpdate() override {

        }
};


int main() {
    srand(time(nullptr));
    BlockWorld app = BlockWorld(); 
    app.run(); 
    return 0;
}
