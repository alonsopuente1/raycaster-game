#ifndef __R_RENDERER_H__
#define __R_RENDERER_H__

#include "texturebank.h"

/* r_renderer.h

    the renderer created is attached to a specific window 
    and contains the texturebank for that window

    rendering of entities should be done through the renderer
    as it include depth buffer test. if not you'll have wallhacks

*/

typedef struct window_s window_t;

typedef struct renderer_s
{

    window_t*       parentWindow;

    float*          depthBuffer;
    // number of elements in depth buffer array
    int             depthBufferCount;

    texturebank_t   textureBank;

} renderer_t;

extern renderer_t R_CreateRenderer(window_t* parentWindow);

// clears depth buffer too
extern void R_ClearScreen(renderer_t* render, SDL_Color colour);
extern void R_Present(renderer_t* render);

extern void R_DestroyRenderer(renderer_t* render);

#endif