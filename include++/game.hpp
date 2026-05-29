#pragma once

#include "window.hpp"
#include "scene.hpp"

#include <unordered_map>
#include <memory>

namespace CastEngine
{
    class Game
    {

    private:

        Window mWindow;
        
        bool running;

        std::unordered_map<std::string, std::unique_ptr<IScene>> mScenes;
        
        IScene* currentScene;

    public:

        void ChangeScene(const std::string& pSceneName);

        Window& GetWindow();

        void ShutDown();

    };

};