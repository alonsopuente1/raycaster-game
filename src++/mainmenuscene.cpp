#include "mainmenuscene.hpp"

#include "window.hpp"
#include "game.hpp"

void MainMenuScene::Setup()
{
    CastEngine::Window& window = mRenderer.GetWindow();
    mRenderer.texBank.PushTexture(CastEngine::Texture(window, "res/maps/map1.sdm"));
}

void MainMenuScene::HandleEvents(SDL_Event e)
{

    if( e.type == SDL_QUIT)
    {
        mParentGame.ShutDown();
        return;
    }

}

void MainMenuScene::Update(float dtMs)
{
}

void MainMenuScene::Draw()
{

}

void MainMenuScene::Destroy()
{
}