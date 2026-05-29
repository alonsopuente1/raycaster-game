#pragma once

#include <SDL2/SDL.h>

namespace CastEngine
{

    class Game;
    /// @brief scene class interface to implement own scenes
    class IScene {
    
    protected:

        Game& mParentGame;

    public:

        IScene(Game& pGame) : mParentGame(pGame) {} 
        
        virtual void Setup() = 0;
        virtual void HandleEvents(SDL_Event e) = 0;
        virtual void Update(float dtMs) = 0;
        virtual void Draw() = 0;
        virtual void Destroy() = 0;

    };

};