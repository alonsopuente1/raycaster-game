#ifndef __M_GAMESTATE_H__
#define __M_GAMESTATE_H__

#include <SDL2/SDL.h>

typedef struct maingame_s maingame_t;

typedef enum maingamestate_e
{
    STATE_NONE = 0,
    STATE_MAINMENU,
    STATE_GAME

} maingamestate_t;

extern void G_ChangeGamestate(maingame_t* game, maingamestate_t newGamestate);

extern void G_HandleEventsMainMenu(maingame_t* game, SDL_Event* e);
extern void G_HandleEventsGame(maingame_t* game, SDL_Event* e);

extern void G_UpdateGame(maingame_t* game, float dt);

extern void G_DrawMainMenu(maingame_t* game);
extern void G_DrawGame(maingame_t* game);
#endif