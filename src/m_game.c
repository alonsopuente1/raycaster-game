#include "m_game.h"

#include <direct.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "logger.h"

#include "r_draw.h"
#include "p_funcs.h"
#include "v_funcs.h"
#include "settings.h"
#include "fonts.h"

#ifdef _WIN32
#include <Windows.h>
#endif

// File paths for all textures to be loaded
static const char* texturePaths[] = {
    "res/textures/env/bluestone.png",
    "res/textures/env/colorstone.png",
    "res/textures/env/greystone.png",
    "res/textures/env/mossy.png",
    "res/textures/env/purplestone.png",
    "res/textures/env/redbrick.png",
    "res/textures/env/wood.png",
    "res/textures/env/eagle.png",
    "res/textures/player.png",
    "res/textures/guns/PIST2.png",
    "res/textures/guns/FIST.png"
};

int NUMTEXTURES = ((int)(sizeof(texturePaths) / sizeof(texturePaths[0])));

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

// this should only be called by G_Init. will return false if textures are already initialised
bool G_InitTextures(maingame_t* game)
{
    if(game->texturebank.headNode)
    {
        LogMsg(WARN, "Textures already initialised! Skipping...\n");
        return false;
    }


    for(int i = 0; i < NUMTEXTURES; i++)
        if(!TB_PushTexture(&game->window, &game->texturebank, texturePaths[i]))
            LogMsgf(ERROR, "failed to load texture at path '%s'", texturePaths[i]);

    // create a blank texture for the minimap
    texture_t* minimapTex = TB_AddEmptyTexture(&game->texturebank);
    if(!minimapTex)
    {
        LogMsg(ERROR, "failed to create minimap texture\n");
        return false;
    }


    *minimapTex = T_CreateBlankTexture(&game->window, "MINIMAP", 256, 256);

    if(minimapTex->data == NULL)
    {
        LogMsgf(ERROR, "Failed to create blank minimap texture. IMG_ERROR: %s\n", IMG_GetError());
        return false;
    }

    LogMsg(INFO, "Textures initialised successfully\n");

    return true;
}

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

bool G_InitPlayer(maingame_t* game)
{
    game->player.pos         = V_Make(4.0f, 4.0f);
    game->player.viewAngle   = 0.0f;
    game->player.maxMoveSpeed= MOVESPEED;
    game->player.rotateSpeed = ROTATESPEED;
    game->player.footstepSoundCooldown = 500.f;
    
    return true;
}

bool G_InitAudio(maingame_t* game)
{
    game->footstepSound1 = Mix_LoadWAV("res/sound/footsteps/1.wav");
    if(!game->footstepSound1)
    {
        LogMsgf(ERROR, "Failed to load footstep1 sound. MIX_ERROR: %s\n", Mix_GetError());
        return false;
    }

    game->footstepSound2 = Mix_LoadWAV("res/sound/footsteps/2.wav");
    if(!game->footstepSound2)
    {
        LogMsgf(ERROR, "Failed to load footstep2 sound. MIX_ERROR: %s\n", Mix_GetError());
        Mix_FreeChunk(game->footstepSound1);
        game->footstepSound1 = NULL;
        return false;
    }

    LogMsg(INFO, "Audio initialised successfully\n"); 
    return true;
}

void G_Init(maingame_t* game)
{
    if(!game)
    {
        LogMsg(ERROR, "passing null ptr to game\n");
        return;
    }

    G_InitLibs();
    G_InitWindow(game);
    FontInit();

    game->gameState = STATE_NONE;

    G_ChangeGamestate(game, STATE_MAINMENU);

    game->running = true;
}

void G_HandleEvents(maingame_t* game)
{
    SDL_Event e;

    while(SDL_PollEvent(&e))
    {
        switch(game->gameState)
        {
        case STATE_MAINMENU:
            G_HandleEventsMainMenu(game, &e);
            break;
        case STATE_GAME:
            G_HandleEventsGame(game, &e);       
        }
    }
}

void G_Update(maingame_t* game, float dt)
{
    float fps = 1000.f / dt;
    char newTitle[64];
    snprintf(newTitle, 63, "RayCaster %.0ffps", fps);
    SDL_SetWindowTitle(game->window.sdlWindow, newTitle);

    switch(game->gameState)
    {
    case STATE_GAME:
        G_UpdateGame(game, dt);
        break;
    default:
        break;
    }
}

void G_Draw(maingame_t* game)
{
    switch(game->gameState)
    {
    case STATE_MAINMENU:
        G_DrawMainMenu(game);
        break;
    case STATE_GAME:
        G_DrawGame(game);
        break;
    default:
        break;
    }
}

void G_Destroy(maingame_t* game)
{
    M_Free(&game->map);

    if(game->footstepSound1)
        Mix_FreeChunk(game->footstepSound1);
    if(game->footstepSound2)
        Mix_FreeChunk(game->footstepSound2);

    if(game->screenButtons)
        free(game->screenButtons);

    TB_FreeAllTextures(&game->texturebank);

    FreeLogs();

    FontCleanup();
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}