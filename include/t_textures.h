#ifndef __TEXTURES_H__
#define __TEXTURES_H__

#include <SDL2/SDL.h>

typedef struct texture_s
{
    int         width;
    int         height;
    SDL_Texture*  data;   // RGBA format
} texture_t;

extern texture_t T_LoadTexture(const char* path);

#endif