#ifndef __W_BUTTONS_H__
#define __W_BUTTONS_H__

#include <SDL2/SDL.h>

/* w_buttons.h

    contains struct and helper functions for implementing 
    buttons on the window

    currently the rresponse to a button click has to be handled 
    within the scene

    TODO: implement a callback function which handles response to button clicks
    TODO: change button colour when mouse is hovering over button

*/

typedef struct renderer_s renderer_t;
typedef struct texture_s texture_t;

typedef struct winButton_s
{
    // position and size of the button on screen
    SDL_Rect    rect;

    // colour of background
    SDL_Color   backgroundColor;
    
    texture_t*  text;

    renderer_t* parentRenderer;

} winButton_t;

extern winButton_t W_CreateButton(renderer_t* render, SDL_Rect rect, SDL_Color backgroundColour);
extern void W_SetButtonText(winButton_t* button, const char* src);
extern void W_SetButtonRect(winButton_t* button, SDL_Rect src);
// since button already has parent window, it can draw
extern void W_DrawButton(winButton_t* button);
extern void W_DestroyButton(winButton_t* button);

#endif