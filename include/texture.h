#ifndef __TEXTURES_H__
#define __TEXTURES_H__

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct texture_s
{
    int             width;
    int             height;
    SDL_Texture*    data;
    char            name[32]; // name of the texture (stripped from file path or set by user if created with T_CreateBlankTexture)
} texture_t;

typedef struct window_s window_t;

// Loads a texture from file path. On failure, returns a texture with data = NULL on failure
extern texture_t T_LoadTexture(window_t* window, const char* path);
// creates a texture intended for drawing to the texture 
// returns texture with texture_t.data == NULL on failure, not NULL on success
extern texture_t T_CreateBlankTexture(window_t* window, const char* name, int width, int height);
extern bool T_SetTextureName(texture_t* tex, const char* name);
extern void T_FreeTexture(texture_t* tex);

#endif 