#ifndef __FONTS_H__
#define __FONTS_H__

#include "texture.h"
#include "m_game.h"

#include <SDL2/SDL_ttf.h>

extern TTF_Font* fonts[64]; 

extern void FontInit();

// appends it to the texturebank of the game
// returns pointer to a texture of the text, NULL on failure
extern texture_t* F_CreateText(maingame_t* dst, SDL_Color colour, TTF_Font* font, const char* text);

extern void FontCleanup();

#endif