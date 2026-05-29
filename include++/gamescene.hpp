#pragma once

#include "scene.hpp"

namespace CastEngine
{

    class GameScene : public IScene
    {

    public:

        void Setup() override;
        void HandleEvents(SDL_Event& e) override;
        void Update(float dtMs) override;
        void Draw() override;
        void Destroy() override;

    };

};