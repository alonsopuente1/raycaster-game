#ifndef __M_STATE_H__
#define __M_STATE_H__

#include <SDL2/SDL.h>

/* m_state.h

    contains the gamestate management functions and enums
    only used to switch between different gamestates

*/
typedef struct maingame_s maingame_t;

typedef enum maingamestate_e
{
    STATE_NONE = 0,
    STATE_MAINMENU,
    STATE_GAME

} maingamestate_t;

// changing gamestates always cleans up all resources before setting up new gamestate
// so no need to clean up resources before
extern void G_ChangeGamestate(maingame_t* game, maingamestate_t newGamestate);
#endif