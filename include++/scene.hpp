#pragma once

#include <SDL2/SDL.h>

namespace CastEngine
{

    /// @brief scene class interface to implement own scenes
    class IScene {
    
    public:

        virtual void Setup();
        virtual void HandleEvents(SDL_Event& e);
        virtual void Update(float dtMs);
        virtual void Draw();
        virtual void Destroy();

    };

};