#ifndef __TEXTURES_H__
#define __TEXTURES_H__

#include <SDL2/SDL.h>

typedef struct texture_s
{
    int             width;
    int             height;
    SDL_Texture*    data;
    char            name[32]; // name of the texture (stripped from file path)
} texture_t;

typedef struct window_s window_t;

// Loads a texture from file path. On failure, returns a texture with data = NULL
extern texture_t T_LoadTexture(window_t* window, const char* path);
extern texture_t T_CreateBlankTexture(window_t* window, const char* name, int width, int height);
extern void T_FreeTexture(texture_t* tex);

#endif 