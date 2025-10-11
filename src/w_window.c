#include "w_window.h"

#include "logger.h"

bool W_InitWindow(window_t* window, const char* title, int width, int height)
{
    if(!window)
    {
        LogMsgf(WARN, "window pointer for window %s is null\n", title);
        return false;
    }

    if(window->sdlWindow || window->sdlRenderer)
    {
        LogMsgf(WARN, "Window %s already created\n", title);
        return true;
    }

    window->sdlWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if(!window->sdlWindow)
    {
        LogMsgf(ERROR, "Failed to create window %s. SDL_ERROR: %s\n", title, SDL_GetError());
        return false;
    }

    window->sdlRenderer = SDL_CreateRenderer(window->sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!window->sdlRenderer)
    {
        LogMsgf(ERROR, "Failed to create renderer for window %s. SDL_ERROR: %s\n", title, SDL_GetError());
        SDL_DestroyWindow(window->sdlWindow);
        window->sdlWindow = NULL;
        return false;
    }

    window->width  = width;
    window->height = height;

    return true;
}

void W_DestroyWindow(window_t* window)
{
    if(window->sdlRenderer)
    {
        SDL_DestroyRenderer(window->sdlRenderer);
        window->sdlRenderer = NULL;
    }

    if(window->sdlWindow)
    {
        SDL_DestroyWindow(window->sdlWindow);
        window->sdlWindow = NULL;
    }
}