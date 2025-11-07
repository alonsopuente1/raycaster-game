#ifndef __M_MAINMENUSTATE_H__
#define __M_MAINMENUSTATE_H__

#include <SDL2/SDL.h>

typedef struct maingame_s maingame_t;

extern void G_HandleEventsMainMenu(maingame_t* game, SDL_Event* e);
extern void G_DrawMainMenu(maingame_t* game);


#endif