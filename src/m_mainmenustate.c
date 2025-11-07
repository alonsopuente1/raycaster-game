#include "m_mainmenustate.h"

#include "m_game.h"
#include "m_state.h"
#include "w_buttons.h"

#include "logger.h"

// function only gets called when the state actually changes to mainmenu
// safe to assume game->screenButtons = NULL since game resources always get freed
// just before changing gamestate
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

    int rectHeight = game->window.height / 6;
    int rectWidth = game->window.width / 2;

    SDL_SetRelativeMouseMode(SDL_FALSE);

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
            return;
        }
        if(SDL_PointInRect(&mouseClick, &game->screenButtons[1].rect))
        {
            game->running = false;
            return;
        }
        break;
    }
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