#ifndef __M_GAMESTATE_H__
#define __M_GAMESTATE_H__

#include <SDL2/SDL.h>

typedef struct maingame_s maingame_t;

extern void G_HandleEventsGame(maingame_t* game, SDL_Event* e);
extern void G_UpdateGame(maingame_t* game, float dt);
extern void G_DrawGame(maingame_t* game);

#endif