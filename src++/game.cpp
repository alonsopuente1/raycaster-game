#include "game.hpp"

namespace CastEngine
{
    void Game::ChangeScene(const std::string &pSceneName)
    {
        if(currentScene)
            currentScene->Destroy();

        currentScene = mScenes[pSceneName].get();
        currentScene->Setup();
    }
    Window &Game::GetWindow()
    {
        return mWindow;
    }
    void Game::ShutDown()
    {
        running = false;
    }
};