#include "s_mainmenuscene.h"

#include "m_game.h"
#include "util.h"
#include "logger.h"
#include "s_gameevents.h"
#include "fonts.h"
#include "r_draw.h"

/* FORWARD DECLERATIONS */

void MMS_RefreshMapList(mainMenuScene_t* scene, maingame_t* game);

/* PUBLIC FUNCTIONS*/

void MMS_SetupScene(void* scene, maingame_t* game)
{
    SDL_assert(scene != NULL && game != NULL);
    memset(scene, 0, sizeof(mainMenuScene_t));
    
    mainMenuScene_t* mmScene = (mainMenuScene_t*)scene;

    mmScene->render = R_CreateRenderer(&game->window);
    if(!mmScene->render.parentWindow)
    {
        LogMsg(ERROR, "failed to create renderer for main menu\n");
        game->running = false;
        return;
    }
    

    int rectHeight = game->window.height / 6;
    int rectWidth = game->window.width / 2;

    SDL_SetRelativeMouseMode(SDL_FALSE);

    SDL_Rect buttons[] = {
        (SDL_Rect){ game->window.width / 2 - rectWidth / 2, game->window.height / 2 - rectHeight - 10, rectWidth, rectHeight },
        (SDL_Rect){ game->window.width / 2 - rectWidth / 2, game->window.height / 2 + 10, rectWidth, rectHeight}
    };
    
    mmScene->startButton = W_CreateButton(&mmScene->render, buttons[0], (SDL_Color){0, 0, 0, 100});
    mmScene->exitButton = W_CreateButton(&mmScene->render, buttons[1], (SDL_Color){0, 0, 0, 100});
    
    W_SetButtonText(&mmScene->startButton, "Play");
    W_SetButtonText(&mmScene->exitButton, "Quit");

    MMS_RefreshMapList(mmScene, game);
    
    mmScene->mapFileButtons = calloc(mmScene->numMapFiles, sizeof(winButton_t));
    if(!mmScene->mapFileButtons)
    {
        LogMsg(ERROR, "no memory");
        exit(-1);
    }

    int backButtonWidth = 100;
    int backButtonHeight = 100;

    mmScene->backButton = W_CreateButton(&mmScene->render, (SDL_Rect){backButtonWidth, game->window.height - backButtonHeight, backButtonWidth, backButtonHeight}, (SDL_Color){0, 0, 0, 100});
    W_SetButtonText(&mmScene->backButton, "Back");


    SDL_Rect rect = { 0 };
    for(int i = 0; i < mmScene->numMapFiles; i++)
    {
        char fileName[32] = { 0 };

        fileNameFromPath(mmScene->mapFiles[i], fileName, sizeof(fileName));

        mmScene->mapFileButtons[i] = W_CreateButton(&mmScene->render, rect, (SDL_Color){0, 0, 0, 100});
        W_SetButtonText(&mmScene->mapFileButtons[i], fileName);
    }

    texture_t* mapSelectText = F_CreateText(&mmScene->render, (SDL_Color){255, 255, 255, 255}, fonts[0], "Map Select");
    T_SetTextureName(mapSelectText, "mapselect");
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

    if(event->type == SDL_KEYDOWN)
    {
        if(event->key.keysym.scancode == SDL_SCANCODE_ESCAPE && !event->key.repeat)
        {
            switch(mmScene->state)
            {
                case MAPCHOOSE:
                {
                    mmScene->state = MAINMENU;
                    break;
                }
                default:
                    break;
            }
        }
    }

    if(event->type == SDL_MOUSEBUTTONDOWN)
    {
        switch(mmScene->state)
        {
        case MAINMENU:
        {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            SDL_Point mousePos = (SDL_Point){mouseX, mouseY};

            if(SDL_PointInRect(&mousePos, &mmScene->startButton.rect))
            {
                LogMsg(INFO, "Start button clicked\n");
                mmScene->state = MAPCHOOSE;
                MMS_RefreshMapList(mmScene, game);
                return;
            }
            else if(SDL_PointInRect(&mousePos, &mmScene->exitButton.rect))
            {
                LogMsg(INFO, "Exit button clicked\n");
                game->running = false;
            }
            break;
        }
        case MAPCHOOSE:
        {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            SDL_Point mousePos = (SDL_Point){mouseX, mouseY};

            if(SDL_PointInRect(&mousePos, &mmScene->backButton.rect))
            {
                mmScene->state = MAINMENU;
            }

            for(int i = 0; i < mmScene->numMapFiles; i++)
            {
                if(SDL_PointInRect(&mousePos, &mmScene->mapFileButtons[i].rect))
                {
                    LogMsgf(INFO, "loading map at file path '%s'\n", mmScene->mapFiles[i]);
                    SDL_Event event = { 0 };
                    event.type = SDL_USEREVENT;
                    event.user.code = EVENT_LOADMAP;
                    event.user.data1 = calloc(strlen(mmScene->mapFiles[i]) + 1, sizeof(char));
                    if(!event.user.data1)
                    {
                        LogMsg(ERROR, "failed to allocate memory for loading map\n");
                        exit(-1);
                    }
                    memcpy(event.user.data1, mmScene->mapFiles[i], strlen(mmScene->mapFiles[i]));
                    if(SDL_PushEvent(&event) <= 0)
                    {
                        LogMsg(ERROR, "failed to push load map event to event queue\n");
                        free(event.user.data1);
                        break;
                    }
                    G_ChangeScene(game, "Game");
                    return;
                }
            }
        }
        }
        
    }
}

void MMS_Update(void* scene, maingame_t* game, float dt)
{
    mainMenuScene_t* mmScene = (mainMenuScene_t*)scene;

    int iterations = mmScene->numMapFiles > 10 ? 10 : mmScene->numMapFiles;

    int rectWidth = game->window.width / 5;
    int rectHeight = game->window.height / 20;

    for(int i = 0; i < iterations; i++)
    {
        SDL_Rect rect = {game->window.width / 2 - rectWidth / 2, rectHeight * (5 + i), rectWidth, rectHeight};

        W_SetButtonRect(&mmScene->mapFileButtons[i], rect);
    }
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

    renderer_t* render = &mmScene->render;

    R_ClearScreen(render, (SDL_Color){50, 50, 50, 255});

    switch(mmScene->state)
    {
    case MAINMENU:
    {
        W_DrawButton(&mmScene->startButton);
        W_DrawButton(&mmScene->exitButton);
        break;
    }
    case MAPCHOOSE:
    {
        // draw buttons
        int iterations = mmScene->numMapFiles > 10 ? 10 : mmScene->numMapFiles;
        for(int i = 0; i < iterations; i++)
        {
            W_DrawButton(&mmScene->mapFileButtons[i]);
        }
        W_DrawButton(&mmScene->backButton);
        
        // draw map select text

        texture_t* mapSelectText = TB_FindTextureByName(&mmScene->render.textureBank, "mapselect");
        
        int dstWidth = mapSelectText->width * 10,
            dstHeight = mapSelectText->height * 10;

        R_RenderTexture(&mmScene->render, 
                        mapSelectText, 
                        (SDL_Rect){0, 0, mapSelectText->width, mapSelectText->height},
                        (SDL_Rect){game->window.width / 2 - dstWidth / 2, 10, dstWidth, dstHeight});
    }
    }
    
    R_Present(render);
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

    FreeDynamicArrayOfAllocatedElements((void**)mmScene->mapFiles, mmScene->numMapFiles);
    mmScene->mapFiles = NULL;

    if(mmScene->mapFileButtons)
    {
        free(mmScene->mapFileButtons);
        mmScene->mapFileButtons = NULL;
    }

    R_DestroyRenderer(&mmScene->render);
}

/* PRIVATE FUNCTIONS */

// since this function is only used inside scene code
// its safe to assume scene and game aren't null
void MMS_RefreshMapList(mainMenuScene_t* scene, maingame_t* game)
{
    if(scene->mapFiles)
    {
        FreeDynamicArrayOfAllocatedElements(scene->mapFiles, scene->numMapFiles);
        scene->mapFiles = NULL;
    }

// for the linux version of the function, only the directory to the folder must be passed
// wildcards are not supported
#ifdef _WIN32
    scene->mapFiles = GetAllFilesInDir("./res/maps/*.sdm", &scene->numMapFiles);
#elif defined(__linux__)
    scene->mapFiles = GetAllFilesInDir("./res/maps", &scene->numMapFiles);
#else
    #error "macOS not supported"
#endif
    
    

    if(!scene->mapFiles)
    {
    }
}
