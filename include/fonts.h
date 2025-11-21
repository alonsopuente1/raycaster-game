#ifndef __FONTS_H__
#define __FONTS_H__

#include <SDL2/SDL_ttf.h>

typedef struct texture_s texture_t;
typedef struct renderer_s renderer_t;

extern TTF_Font* fonts[64]; 

extern void FontInit();

// appends it to the texturebank of the renderer
// returns pointer to a texture of the text, NULL on failure
extern texture_t* F_CreateText(renderer_t* renderer, SDL_Color colour, TTF_Font* font, const char* text);

extern void FontCleanup();

#endif