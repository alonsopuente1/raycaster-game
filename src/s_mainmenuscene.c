#include "s_mainmenuscene.h"

#include "m_game.h"

#include "logger.h"

void MMS_SetupScene(void* scene, maingame_t* game)
{
    if(!scene)
    {
        LogMsg(ERROR, "passed null ptr to scene, something is very wrong\n");
        return;
    }

    if(!game)
    {
        LogMsg(ERROR, "passed null ptr to game, something is very wrong\n");
        return;
    }

    mainMenuScene_t* mmScene = (mainMenuScene_t*)scene;

    memset(scene, 0, sizeof(mainMenuScene_t));

    int rectHeight = game->window.height / 6;
    int rectWidth = game->window.width / 2;

    SDL_SetRelativeMouseMode(SDL_FALSE);

    SDL_Rect buttons[] = {
        (SDL_Rect){ game->window.width / 2 - rectWidth / 2, game->window.height / 2 - rectHeight - 10, rectWidth, rectHeight },
        (SDL_Rect){ game->window.width / 2 - rectWidth / 2, game->window.height / 2 + 10, rectWidth, rectHeight}
    };
    
    mmScene->startButton = W_CreateButton(game, buttons[0], (SDL_Color){0, 0, 0, 100});
    mmScene->exitButton = W_CreateButton(game, buttons[1], (SDL_Color){0, 0, 0, 100});
    
    W_SetButtonText(&mmScene->startButton, "Play");
    W_SetButtonText(&mmScene->exitButton, "Quit");
}

void MMS_HandleEvents(void* scene, maingame_t* game, SDL_Event* event)
{
    if(!scene)
    {
        LogMsg(ERROR, "passed null ptr to scene, something is very wrong\n");
        return;
    }

    if(!game)
    {
        LogMsg(ERROR, "passed null ptr to game, something is very wrong\n");
        return;
    }

    if(!event)
    {
        LogMsg(ERROR, "passed null ptr to event\n");
        return;
    }

    mainMenuScene_t* mmScene = (mainMenuScene_t*)scene;

    if(event->type == SDL_MOUSEBUTTONDOWN)
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_Point mousePos = (SDL_Point){mouseX, mouseY};

        if(SDL_PointInRect(&mousePos, &mmScene->startButton.rect))
        {
            LogMsg(INFO, "Start button clicked\n");
            G_ChangeScene(game, "Game");
            return;
        }
        else if(SDL_PointInRect(&mousePos, &mmScene->exitButton.rect))
        {
            LogMsg(INFO, "Exit button clicked\n");
            game->running = false;
        }
    }
}

void MMS_Update(void* scene, maingame_t* game, float dt)
{

}

void MMS_Draw(void* scene, maingame_t* game)
{
    if(!scene)
    {
        LogMsg(ERROR, "passed null ptr to scene, something is very wrong\n");
        return;
    }
    if(!game)
    {
        LogMsg(ERROR, "passed null ptr to game, something is very wrong\n");
        return;
    }

    mainMenuScene_t* mmScene = (mainMenuScene_t*)scene;

    SDL_Renderer* render = game->window.sdlRenderer;

    SDL_SetRenderDrawColor(render, 50, 50, 50, 255);
    SDL_RenderClear(render);

    W_DrawButton(&mmScene->startButton);
    W_DrawButton(&mmScene->exitButton);

    SDL_RenderPresent(render);

}

void MMS_Destroy(void* scene, maingame_t* game)
{
    if(!scene)
    {
        LogMsg(ERROR, "passed null ptr to scene, something is very wrong\n");
        return;
    }

    if(!game)
    {
        LogMsg(ERROR, "passed null ptr to game, something is very wrong\n");
        return;
    }

    mainMenuScene_t* mmScene = (mainMenuScene_t*)scene;

    W_DestroyButton(&mmScene->startButton);
    W_DestroyButton(&mmScene->exitButton);
}