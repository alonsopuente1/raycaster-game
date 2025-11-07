#include "m_state.h"

#include "m_game.h"
#include "v_funcs.h"
#include "settings.h"
#include "p_funcs.h"
#include "r_draw.h"

#include "logger.h"

extern void G_SetupGame(maingame_t* game);
extern void G_SetupMainMenu(maingame_t* game);

void G_ChangeGamestate(maingame_t* game, maingamestate_t newGamestate)
{
    if(!game)
    {
        LogMsg(WARN, "passed null ptr to game\n");
        return;
    }

    if(newGamestate == game->gameState)
    {
        LogMsg(WARN, "attempting to change gamestate to same gamestate\n");
        return;
    }
    
    game->gameState = newGamestate;

    switch(newGamestate)
    {
    case STATE_GAME:
        G_CleanupResources(game);
        G_SetupGame(game);
        break;
    case STATE_MAINMENU:
        G_CleanupResources(game);
        G_SetupMainMenu(game);
        break;
    default:
        LogMsg(ERROR, "attempting to change to an unknown gamestate\n");
        break;
    }
}

