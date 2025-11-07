#include "m_gamestate.h"

#include "m_state.h"
#include "m_game.h"
#include "r_draw.h"
#include "p_funcs.h"

#include "logger.h"

extern bool G_InitPlayer(maingame_t* game);
extern bool G_InitAudio(maingame_t* game);
extern bool G_InitTextures(maingame_t* game);

void G_SetupGame(maingame_t* game)
{
    if(!game)
    {
        LogMsg(WARN, "passed null ptr to game\n");
        return;
    }
    
    if(!game->window.sdlWindow || !game->window.sdlRenderer)
    {
        LogMsg(ERROR, "game window not initialised\n");
        return;
    }

    maploadargs_t mapArgs = { 0 };

    M_LoadMap(&game->map, &mapArgs, "res/maps/map1.sdm");

    if(!mapArgs.success)
    {
        LogMsg(ERROR, "failed to load map\n");
        G_ChangeGamestate(game, STATE_MAINMENU);
        return;
    }

    if(!G_InitAudio(game))
    {
        LogMsg(ERROR, "failed to switch states\n");
        G_ChangeGamestate(game, STATE_MAINMENU);
        return;
    }

    if(!G_InitTextures(game))
    {
        LogMsg(ERROR, "failed to switch states\n");
        G_ChangeGamestate(game, STATE_MAINMENU);
        return;
    }

    if(!G_InitPlayer(game))
    {
        LogMsg(ERROR, "failed to switch states\n");
        G_ChangeGamestate(game, STATE_MAINMENU);
        return;
    }

    SDL_RaiseWindow(game->window.sdlWindow);

    if(SDL_SetRelativeMouseMode(SDL_TRUE) < 0)
        LogMsg(WARN, "failed to constrain mouse within window\n");

    game->player.pos = mapArgs.startPos;
}

void G_HandleEventsGame(maingame_t* game, SDL_Event* e)
{
    if(!e)
        return;
    
    if(!game)
        return;

    if(e->type == SDL_MOUSEMOTION)
        P_Rotate(&game->player, (float)(e->motion.xrel) / 100);

        // will never return null since sdl is initialised
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    
    if(keys[SDL_SCANCODE_ESCAPE])
    {
        G_ChangeGamestate(game, STATE_MAINMENU);
        return;
    }

    if(keys[SDL_SCANCODE_W])
        game->player.moveState |= MOVE_FORWARD;
    else
        game->player.moveState &= ~MOVE_FORWARD;

    if(keys[SDL_SCANCODE_S])
        game->player.moveState |= MOVE_BACKWARD;
    else
        game->player.moveState &= ~MOVE_BACKWARD;

    if(keys[SDL_SCANCODE_A])
        game->player.moveState |= STRAFE_LEFT;
    else
        game->player.moveState &= ~STRAFE_LEFT;

    if(keys[SDL_SCANCODE_D])
        game->player.moveState |= STRAFE_RIGHT;
    else
        game->player.moveState &= ~STRAFE_RIGHT;

    if(keys[SDL_SCANCODE_RIGHT])
        game->player.moveState |= ROTATE_RIGHT;
    else
        game->player.moveState &= ~ROTATE_RIGHT;
        
    if(keys[SDL_SCANCODE_LEFT])
        game->player.moveState |= ROTATE_LEFT;
    else
        game->player.moveState &= ~ROTATE_LEFT;
    
    Uint32 flags = SDL_GetWindowFlags(game->window.sdlWindow);    
}

void G_UpdateGame(maingame_t* game, float dt)
{
    P_HandleState(&game->player, &game->map, dt);

    if(SDL_GetWindowFlags(game->window.sdlWindow) & SDL_WINDOW_INPUT_FOCUS)
        SDL_SetRelativeMouseMode(SDL_TRUE);
    else
        SDL_SetRelativeMouseMode(SDL_FALSE);
}

void G_DrawGame(maingame_t* game)
{
    // Clear screen
    SDL_SetRenderDrawColor(game->window.sdlRenderer, 0, 0, 0, 0xff);
    SDL_RenderClear(game->window.sdlRenderer);
        
    R_RenderCeilingAndFloor(game);
    R_RenderPlayerView(game);
    R_RenderPlayerGun(game);
    R_RenderMinimap(game);
    
    SDL_RenderPresent(game->window.sdlRenderer);
}