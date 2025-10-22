#include "m_gamestate.h"

#include "m_game.h"
#include "v_funcs.h"
#include "settings.h"
#include "p_funcs.h"
#include "r_draw.h"

#include "logger.h"

extern bool G_InitPlayer(maingame_t* game);
extern bool G_InitAudio(maingame_t* game);
extern bool G_InitTextures(maingame_t* game);

void G_SetupGame(maingame_t* game);
void G_SetupMainMenu(maingame_t* game);

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
        G_SetupGame(game);
        break;
    case STATE_MAINMENU:
        G_SetupMainMenu(game);
        break;
    default:
        LogMsg(ERROR, "attempting to change to an unknown gamestate\n");
        break;
    }
}

// function only gets called when the state actually changes to mainmenu
// safe to assume game->screenButtons = NULL
void G_SetupMainMenu(maingame_t* game)
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

    TB_FreeAllTextures(&game->texturebank);
    Mix_FreeChunk(game->footstepSound1);
    Mix_FreeChunk(game->footstepSound2);
    M_Free(&game->map);
    if(game->screenButtons)
        free(game->screenButtons);

    int rectHeight = game->window.height / 6;
    int rectWidth = game->window.width / 2;

    SDL_Rect buttons[] = {
        (SDL_Rect){ game->window.width / 2 - rectWidth / 2, game->window.height / 2 - rectHeight - 10, rectWidth, rectHeight },
        (SDL_Rect){ game->window.width / 2 - rectWidth / 2, game->window.height / 2 + 10, rectWidth, rectHeight}
    };
    
    game->numButtons = sizeof(buttons) / sizeof(buttons[0]);
    game->screenButtons = calloc(game->numButtons, sizeof(winButton_t));

    game->screenButtons[0] = W_CreateButton(game, buttons[0], (SDL_Color){0, 0, 0, 100});
    game->screenButtons[1] = W_CreateButton(game, buttons[1], (SDL_Color){0, 0, 0, 100});

    W_SetButtonText(&game->screenButtons[0], "Play");
    W_SetButtonText(&game->screenButtons[1], "Quit");
}

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

    if(game->screenButtons)
        free(game->screenButtons);

    game->numButtons = 0;

    maploadargs_t mapArgs = { 0 };

    TB_FreeAllTextures(&game->texturebank);

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
        M_Free(&game->map);
        G_ChangeGamestate(game, STATE_MAINMENU);
        return;
    }

    if(!G_InitTextures(game))
    {
        LogMsg(ERROR, "failed to switch states\n");
        M_Free(&game->map);
        Mix_FreeChunk(game->footstepSound1);
        Mix_FreeChunk(game->footstepSound2);
        G_ChangeGamestate(game, STATE_MAINMENU);
        return;
    }

    if(!G_InitPlayer(game))
    {
        LogMsg(ERROR, "failed to switch states\n");
        G_ChangeGamestate(game, STATE_MAINMENU);
        M_Free(&game->map);
        Mix_FreeChunk(game->footstepSound1);
        Mix_FreeChunk(game->footstepSound2);
        G_ChangeGamestate(game, STATE_MAINMENU);
        return;
    }

    game->player.pos = mapArgs.startPos;
}

void G_HandleEventsGame(maingame_t* game, SDL_Event* e)
{
    if(!e)
        return;
    
    if(!game)
        return;

    const Uint8* keys = SDL_GetKeyboardState(NULL);
    
    if(keys[SDL_SCANCODE_ESCAPE])
    {
        game->running = false;
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

    if(flags & SDL_WINDOW_INPUT_FOCUS)
    {
        int x, _;
        SDL_GetMouseState(&x, &_);

        int deltaX = x - game->window.width / 2;

        P_Rotate(&game->player, deltaX * game->player.rotateSpeed);
        SDL_WarpMouseInWindow(game->window.sdlWindow, game->window.width / 2, game->window.height / 2);

        SDL_ShowCursor(SDL_DISABLE);
    }
    else
    {
        SDL_ShowCursor(SDL_ENABLE);
    }
}

void G_HandleEventsMainMenu(maingame_t* game, SDL_Event* e)
{
    if(!e)
        return;

    if(!game)
        return;

    if(!game->screenButtons)
        return;

    switch(e->type)
    {
    case SDL_QUIT:
        game->running = false;
        break;
    case SDL_MOUSEBUTTONDOWN:
        SDL_Point mouseClick = (SDL_Point){e->motion.x, e->motion.y};
        if(SDL_PointInRect(&mouseClick, &game->screenButtons[0].rect))
        {
            G_ChangeGamestate(game, STATE_GAME);
        }
        if(SDL_PointInRect(&mouseClick, &game->screenButtons[1].rect))
        {
            game->running = false;
        }
        break;
    }
}

void G_UpdateGame(maingame_t* game, float dt)
{
    P_HandleState(&game->player, &game->map, dt);
}

void G_DrawMainMenu(maingame_t* game)
{
    if(!game)
        return;

    if(!game->window.sdlRenderer || !game->window.sdlWindow)
        return;

    SDL_Renderer* render = game->window.sdlRenderer;

    SDL_SetRenderDrawColor(render, 0, 0, 0xff, 0xff);
    SDL_RenderClear(render);

    for(int i = 0; i < game->numButtons; i++)
        W_DrawButton(&game->screenButtons[i]);

    SDL_RenderPresent(render);
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