#ifndef __W_BUTTONS_H__
#define __W_BUTTONS_H__

#include <SDL2/SDL.h>

typedef struct maingame_s maingame_t;
typedef struct texture_s texture_t;
typedef struct winButton_s
{

    SDL_Rect    rect;
    SDL_Color   backgroundColor;

    texture_t*  text;

    maingame_t* parentGame;

} winButton_t;

extern winButton_t W_CreateButton(maingame_t* game, SDL_Rect rect, SDL_Color colour);
extern void W_SetButtonText(winButton_t* button, const char* src);
extern void W_SetButtonRect(winButton_t* button, SDL_Rect src);
// since button already has parent window, it can draw
extern void W_DrawButton(winButton_t* button);

#endif