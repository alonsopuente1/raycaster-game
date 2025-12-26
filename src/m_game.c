#include "m_game.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "logger.h"

#include "r_draw.h"
#include "p_funcs.h"
#include "v_funcs.h"
#include "settings.h"
#include "fonts.h"

#if _WIN32 
#include <Windows.h>
#endif

/* ----   FORWARD DECLARATIONS      ---- */
void G_HandleEvents(maingame_t* game);
void G_Update(maingame_t* game, float dt);
void G_Draw(maingame_t* game);
bool G_InitLibs();
bool G_InitWindow(maingame_t* game);

/* ----     PUBLIC FUNCTIONS        ---- */
void G_Run(maingame_t* game)
{
    uint64_t oldTime = SDL_GetTicks64();

    while(game->running)
    {
        uint64_t newTime = SDL_GetTicks64();
        uint64_t deltaTime = newTime - oldTime;
        oldTime = SDL_GetTicks64();

        G_HandleEvents(game);
        G_Update(game, (float)deltaTime);
        G_Draw(game);
    }
}

bool G_Init(maingame_t* game)
{
    if(!game)
    {
        LogMsg(ERROR, "passing null ptr to game\n");
        return false;
    }

    if(!G_InitLibs())
    {
        LogMsg(ERROR, "failed to initialise libraries\n");
        return false;
    }

    if(!G_InitWindow(game))
    {
        LogMsg(ERROR, "failed to initalise game\n");
        return false;
    }

    FontInit();

    game->currentScene = NULL;
    game->running = true;

    return true;
}

void G_Destroy(maingame_t* game)
{
    if(!game)
        return;

    if(game->scenes)
    {
        for(int i = 0; i < game->numScenes; i++)
        {
            S_DestroyScene(&game->scenes[i]);
        }

        free(game->scenes);
        game->scenes = NULL;
        game->numScenes = 0;
        game->currentScene = NULL;
    }
    
    game->running = false;

    FontCleanup();
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

bool G_AddScene(maingame_t* game, void* scene, unsigned int size, const char* sceneName)
{

    if(!game)
    {
        LogMsg(ERROR, "passed null ptr to game\n");
        return false;
    }

    scene_t newScene = S_CreateScene(game, scene, size, sceneName);
    if(!newScene.data)
    {
        LogMsg(ERROR, "failed to create scene\n");
        return false;
    }

    scene_t* newScenesArray = realloc(game->scenes, sizeof(scene_t) * (game->numScenes + 1));
    if(!newScenesArray)
    {
        LogMsg(ERROR, "failed to realloc scenes array to add new scene\n");
        exit(-1);
    }

    game->scenes = newScenesArray;
    game->scenes[game->numScenes] = newScene;
    game->numScenes++;

    return true;
}

void G_AddSceneExisting(maingame_t* game, scene_t* scene)
{
    if(!game)
    {
        LogMsg(ERROR, "passed null ptr to game\n");
        return;
    }

    if(!scene)
    {
        LogMsg(ERROR, "passed null ptr to scene\n");
        return;
    }

    if(game != scene->parentGame)
    {
        LogMsg(ERROR, "scene's parentGame does not match the passed game pointer\n");
        return;
    }

    scene_t* newScenesArray = realloc(game->scenes, sizeof(scene_t) * (game->numScenes + 1));
    if(!newScenesArray)
    {
        LogMsg(ERROR, "failed to realloc scenes array to add new scene\n");
        exit(-1);
    }

    game->scenes = newScenesArray;
    game->scenes[game->numScenes] = *scene;
    game->numScenes++;
}

void G_ChangeScene(maingame_t* game, const char* sceneName)
{
    if(!game)
    {
        LogMsg(ERROR, "passed null ptr to game\n");
        return;
    }

    if(!sceneName)
    {
        LogMsg(ERROR, "passed null ptr to sceneName\n");
        return;
    }

    for(int i = 0; i < game->numScenes; i++)
    {
        if(strcmp(game->scenes[i].sceneName, sceneName) == 0)
        {
            LogMsgf(INFO, "Changing to scene '%s'\n", sceneName);
            if(game->currentScene)
                S_CallDestroy(game->currentScene);
            game->currentScene = &game->scenes[i];
            S_CallSetup(game->currentScene);
            return;
        }
    }

    LogMsgf(ERROR, "Scene '%s' not found! Cannot change scene\n", sceneName);
}


/*  -----------------
    PRIVATE FUNCTIONS
    ----------------- */
bool G_InitLibs()
{
    if(!IMG_Init(IMG_INIT_PNG))
    {
        LogMsgf(ERROR, "Failed to initialise SDL_image. Error: %s\n\tGetting out of here...\n", IMG_GetError());
        return false;
    }
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        LogMsgf(ERROR, "Failed to init SDL2. SDL_ERROR: %s\n\tGetting out of here...\n", SDL_GetError());
        return false;
    }
    if(Mix_Init(MIX_INIT_MP3 | MIX_INIT_WAVPACK) == 0)
    {
        LogMsgf(ERROR, "Failed to initialise SDL_mixer. MIX_ERROR: %s\n\tGetting out of here...\n", Mix_GetError());
        return false;
    }
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        LogMsgf(ERROR, "Failed to initialise SDL_Mixer. MIX_ERROR: %s\n\tGetting out of here...\n", Mix_GetError());
        return false;
    }
    if(TTF_Init() < 0)
    {
        LogMsgf(ERROR, "failed to initialise TTF_Fonts. TTF_ERROR: %s\n", TTF_GetError());
        return false;
    }

    return true;
}

bool G_InitWindow(maingame_t* game)
{
    if(!W_InitWindow(&game->window, "RayCaster", 1280, 720))
    {
        LogMsg(ERROR, "FATAL: failed to create main window\n");
        return false;
    }
    LogMsg(INFO, "Main window initialised successfully\n");


    return true;
}

void G_HandleEvents(maingame_t* game)
{
    if(!game->currentScene)
    {
        LogMsg(ERROR, "no current scene to handle events!\n");
        return;
    }

    SDL_Event e;

    while(SDL_PollEvent(&e))
    {
        if(e.type == SDL_QUIT)
        {
            game->running = false;
            return;
        }        
        S_CallHandleEvents(game->currentScene, &e);
    }

    
}

void G_Update(maingame_t* game, float dt)
{
    if(!game->currentScene)
    {
        LogMsg(ERROR, "no current scene to update!\n");
        return;
    }

    float fps = 1000.f / dt;
    char newTitle[64];
    snprintf(newTitle, 63, "RayCaster %.0ffps", fps);
    SDL_SetWindowTitle(game->window.sdlWindow, newTitle);

    S_CallUpdate(game->currentScene, dt);
}

void G_Draw(maingame_t* game)
{
    if(!game->currentScene)
    {
        LogMsg(ERROR, "no current scene to draw!\n");
        return;
    }

    S_CallDraw(game->currentScene);
}