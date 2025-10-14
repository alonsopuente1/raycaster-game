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

// Loads a texture from file path. On failure, returns a texture with data = NULL
extern texture_t T_LoadTexture(const char* path);
extern texture_t T_CreateBlankTexture(const char* name, int width, int height);
extern void T_FreeTexture(texture_t* tex);
// returns NULL if not found
extern texture_t* T_FindTexture(const char* texName);
extern texture_t* gTextures;

#endif