#ifndef __R_RENDERER_H__
#define __R_RENDERER_H__

#include "texturebank.h"

typedef struct window_s window_t;

typedef struct renderer_s
{

    window_t*       parentWindow;

    float*          depthBuffer;

    texturebank_t   textureBank;

} renderer_t;

extern renderer_t R_CreateRenderer(window_t* parentWindow);
extern void R_DestroyRenderer(renderer_t* render);

#endif