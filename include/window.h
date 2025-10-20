#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct window_s
{
    int             width;
    int             height; 

    SDL_Window*     sdlWindow;
    SDL_Renderer*   sdlRenderer;

} window_t;

// returns true on success, false on failure
bool W_InitWindow(window_t* window, const char* title, int width, int height);

// safe to pass null
void W_DestroyWindow(window_t* window);

#endif