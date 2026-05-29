#pragma once

#include "renderer.hpp"
#include "player.hpp"
#include "scene.hpp"

class MainMenuScene : public CastEngine::IScene
{

public:
    void Setup() override;
    void HandleEvents(SDL_Event e) override;
    void Update(float dtMs) override;
    void Draw() override;
    void Destroy() override;
private:

    CastEngine::Renderer mRenderer;



};
