#ifndef __W_WINDOW_H__
#define __W_WINDOW_H__

#include <SDL2/SDL.h>
#include <stdbool.h>

/* w_window.h

    contains window definition and helper funcs

*/

typedef struct window_s
{
    int             width;
    int             height; 

    SDL_Window*     sdlWindow;
    SDL_Renderer*   sdlRenderer;

} window_t;

// returns true on success, false on failure
extern bool W_InitWindow(window_t* window, const char* title, int width, int height);

// returns true if window initialised, false if not
extern bool W_Initialised(window_t* window);

// safe to pass null
extern void W_DestroyWindow(window_t* window);

#endif