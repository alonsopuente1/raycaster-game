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

    return true;
}

bool G_InitGraphics(maingame_t* game)
{
    if(!W_InitWindow(&game->window, "RayCaster", 1280, 720))
    {
        LogMsg(ERROR, "FATAL: failed to create main window\n");
        return false;
    }
    LogMsg(INFO, "Graphics initialised successfully\n");

    return true;
}

void G_InitPlayer(maingame_t* game)
{
    game->player.pos         = V_Make(4.0f, 4.0f);
    game->player.viewAngle   = 0.0f;
    game->player.maxMoveSpeed= MOVESPEED;
    game->player.rotateSpeed = ROTATESPEED;
    game->player.footstepSoundCooldown = 500.f;
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
    if(game->running)
    {
        LogMsg(WARN, "trying to create maingame when already created\n");
        return;
    }

    if(!game)
    {
        LogMsg(WARN, "passing null for initialising game\n");
        return;
    }

    G_InitLibs();
    G_InitGraphics(game);
    G_InitPlayer(game);
    G_InitAudio(game);
    G_InitTextures(game);

    M_LoadMap(&game->map, "res/maps/map1.sdm");

    game->running = true;
}

void G_HandleEvents(maingame_t* game)
{
    SDL_Event e;

    while(SDL_PollEvent(&e))
    {
        switch(e.type)
        {
        case SDL_QUIT:
            game->running = false;
            return;
        }
    }
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    
    if(keys[SDL_SCANCODE_ESCAPE])
    {
        game->running = false;
        return;
    }

    if(keys[SDL_SCANCODE_W])
        game->player.moveState |= 1;
    else
        game->player.moveState &= ~1;

    if(keys[SDL_SCANCODE_S])
        game->player.moveState |= 1 << 1;
    else
        game->player.moveState &= ~(1 << 1);

    if(keys[SDL_SCANCODE_A])
        game->player.moveState |= 1 << 2;
    else
        game->player.moveState &= ~(1 << 2);

    if(keys[SDL_SCANCODE_D])
        game->player.moveState |= 1 << 3;
    else
        game->player.moveState &= ~(1 << 3);

    if(keys[SDL_SCANCODE_RIGHT])
        game->player.moveState |= 1 << 4;
    else
        game->player.moveState &= ~(1 << 4);
        
    if(keys[SDL_SCANCODE_LEFT])
        game->player.moveState |= 1 << 5;
    else
        game->player.moveState &= ~(1 << 5);
    
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

void G_Update(maingame_t* game, float dt)
{
    float fps = 1000.f / dt;
    char newTitle[64];
    snprintf(newTitle, 63, "RayCaster %.0ffps", fps);
    SDL_SetWindowTitle(game->window.sdlWindow, newTitle);

    P_HandleState(&game->player, &game->map, dt);
}

void G_Draw(maingame_t* game)
{
    // Clear screen
    SDL_SetRenderDrawColor(game->window.sdlRenderer, 0, 0, 0, 0xff);
    SDL_RenderClear(game->window.sdlRenderer);
        
    R_RenderCeilingAndFloor(&game->window);
    R_RenderPlayerView(&game->window, &game->texturebank, &game->player, &game->map);
    R_RenderPlayerGun(&game->window, &game->texturebank, &game->player);
    R_RenderMinimap(&game->window, &game->texturebank, &game->player, &game->map);
    
    SDL_RenderPresent(game->window.sdlRenderer);
}

void G_Destroy(maingame_t* game)
{
    M_Free(&game->map);

    if(game->footstepSound1)
        Mix_FreeChunk(game->footstepSound1);
    if(game->footstepSound2)
        Mix_FreeChunk(game->footstepSound2);

    TB_FreeAllTextures(&game->texturebank);

    FreeLogs();

    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}