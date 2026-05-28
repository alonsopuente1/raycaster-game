#include "window.hpp"

#include "logger.h"

namespace CastEngine
{
    Window::Window() : mSDLWindow(NULL), mSDLRenderer(NULL)
    {
        
    }

    Window::Window(const std::string& title, int pWidth, int pHeight)
    {
        mSDLWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, pWidth, pHeight, 0);
        if(mSDLWindow == NULL)
        {
            LogMsg(ERROR, "failed to create window. SDL_ERROR: %s", SDL_GetError());
            return;
        }

        mSDLRenderer = SDL_CreateRenderer(mSDLWindow, -1, 0);
        if(mSDLRenderer == NULL)
        {
            LogMsg(ERROR, "failed to create renderer. SDL_ERROR: %s", SDL_GetError());
            SDL_DestroyWindow(mSDLWindow);            
            return;
        }

        
    }
    
    // the following macro is used for necessary checking for proper
    // creation of window before using methods

    #define CHECKWINDOW(window) if(window == NULL) { LogMsg(WARN, "passed null ptr to window"); return;}
    #define CHECKRENDER(render) if(render == NULL) { LogMsg(WARN, "passed null ptr to renderer"); return;}

    void Window::SetTitle(const std::string& newTitle)
    {
        CHECKWINDOW(mSDLWindow);

        SDL_SetWindowTitle(mSDLWindow, newTitle.c_str());
    }

    const char* Window::GetTitle() const
    {
        CHECKWINDOW(mSDLWindow);

        return SDL_GetWindowTitle(mSDLWindow);
    }
    
    int Window::GetWidth() const
    {   
        CHECKWINDOW(mSDLWindow);

        int width;
        SDL_GetWindowSize(mSDLWindow, &width, NULL);
        return width;
    }
    int Window::GetHeight() const
    {
        CHECKWINDOW(mSDLWindow);

        int height;
        SDL_GetWindowSize(mSDLWindow, NULL, &height);
        return height;
    }

    SDL_Renderer *Window::GetRenderer() const
    {
        CHECKRENDER(mSDLRenderer);

        return mSDLRenderer;
    }
    SDL_Window *Window::GetWindow() const
    {
        CHECKWINDOW(mSDLWindow);

        return mSDLWindow;
    }

    #undef CHECKWINDOW
    #undef CHECKRENDER
};