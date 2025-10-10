#ifndef __TEXTURES_H__
#define __TEXTURES_H__

#include <SDL2/SDL.h>

typedef struct texture_s
{
    int         width;
    int         height;
    SDL_Texture*  data;
} texture_t;

// Loads a texture from file path. On failure, returns a texture with data = NULL
extern texture_t T_LoadTexture(const char* path);

extern void T_FreeTexture(texture_t* tex);

extern texture_t* gTextures;

#endif