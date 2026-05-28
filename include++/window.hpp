#pragma once

#include <SDL2/SDL.h>
#include <stdint.h>
#include <string>

namespace CastEngine
{

    class Window
    {

    private:

        SDL_Window* mSDLWindow;
        SDL_Renderer* mSDLRenderer;

    public:
    
        Window();
        Window(const std::string& title, int pWidth, int pHeight);


        void SetTitle(const std::string& newTitle);
        const char* GetTitle() const;

        int GetWidth() const;
        int GetHeight() const;

        SDL_Renderer* GetRenderer() const;
        SDL_Window* GetWindow() const;
    };

};