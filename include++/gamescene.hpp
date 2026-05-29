#pragma once

#include "scene.hpp"
#include "renderer.hpp"
#include "window.hpp"


class GameScene : public CastEngine::IScene
{

public:

    void Setup() override;
    void HandleEvents(SDL_Event e) override;
    void Update(float dtMs) override;
    void Draw() override;
    void Destroy() override;


private:

    

};
