#include "window.hpp"

#include "logger.hpp"

namespace CastEngine
{
    Window::Window() : mSDLWindow(NULL), mSDLRenderer(NULL)
    {
        
    }

    Window::Window(const std::string& title, int pWidth, int pHeight)
    {
        CreateWindow(title, pWidth, pHeight);
    }

    Window::~Window()
    {
        Destroy();
    }

    void Window::Destroy()
    {
        if(mSDLWindow)
            SDL_DestroyWindow(mSDLWindow);

        if(mSDLRenderer)
            SDL_DestroyRenderer(mSDLRenderer);

        mSDLWindow = NULL;
        mSDLRenderer = NULL;
    }

    bool Window::CreateWindow(const std::string &title, int pWidth, int pHeight)
    {
        if(IsInitialised())
            Destroy();
        mSDLWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, pWidth, pHeight, 0);
        if(mSDLWindow == NULL)
        {
            LogMsgf(ERROR, "failed to create window. SDL_ERROR: %s", SDL_GetError());
            return false;
        }

        mSDLRenderer = SDL_CreateRenderer(mSDLWindow, -1, 0);
        if(mSDLRenderer == NULL)
        {
            LogMsgf(ERROR, "failed to create renderer. SDL_ERROR: %s", SDL_GetError());
            Destroy();          
            return false;
        }

        return true;
    }

    void Window::SetTitle(const std::string& newTitle)
    {
        if(!IsInitialised())
        {
            LogMsg(WARN, "attempting to set unintialised window title");
            return;
        }

        SDL_SetWindowTitle(mSDLWindow, newTitle.c_str());
    }

    const char* Window::GetTitle() const
    {
        if(!IsInitialised())
        {
            LogMsg(WARN, "attempting to get unintialised window title");
            return NULL;
        }
        return SDL_GetWindowTitle(mSDLWindow);
    }
    
    int Window::GetWidth() const
    {   
        if(!IsInitialised())
        {
            LogMsg(WARN, "attempting to get unintialised window width");
            return 0;
        }
        int width;
        SDL_GetWindowSize(mSDLWindow, &width, NULL);
        return width;
    }

    int Window::GetHeight() const
    {
        if(!IsInitialised())
        {
            LogMsg(WARN, "attempting to get unintialised window height");
            return 0;
        }

        int height;
        SDL_GetWindowSize(mSDLWindow, NULL, &height);
        return height;
    }

    SDL_Renderer *Window::GetRenderer() const
    {
        if(!IsInitialised())
        {
            LogMsg(WARN, "attempting to get unintialised window SDL_Renderer");
            return nullptr;
        }
    
        return mSDLRenderer;
    }
    SDL_Window *Window::GetWindow() const
    {
        if(!IsInitialised())
        {
            LogMsg(ERROR, "attempting to get uninitialised window SDL_Window");
            return nullptr;
        }
        
        return mSDLWindow;
    }

    bool Window::IsInitialised() const
    {
        return mSDLWindow != NULL && mSDLRenderer != NULL;
    }

};