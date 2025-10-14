#include "i_init.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>

#include "settings.h"
#include "p_player.h"
#include "v_funcs.h"
#include "m_map.h"
#include "t_textures.h"
#include "w_window.h"

#include "logger.h"

// File paths for all textures to be loaded
static const char* gTexturePaths[] = {
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

int NUMTEXTURES = ((int)(sizeof(gTexturePaths) / sizeof(gTexturePaths[0])));

extern window_t gMainWindow;
extern player_t gPlayer;
extern map_t gMap;

extern Mix_Chunk* gFootstep1;
extern Mix_Chunk* gFootstep2;

void I_InitLibs()
{
    if(!IMG_Init(IMG_INIT_PNG))
    {
        LogMsgf(ERROR, "Failed to initialise SDL_image. Error: %s\n\tGetting out of here...\n", IMG_GetError());
        exit(-1);
    }
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        LogMsgf(ERROR, "Failed to init SDL2. SDL_ERROR: %s\n\tGetting out of here...\n", SDL_GetError());
        exit(-1);
    }
    if(Mix_Init(MIX_INIT_MP3 | MIX_INIT_WAVPACK) == 0)
    {
        LogMsgf(ERROR, "Failed to initialise SDL_mixer. MIX_ERROR: %s\n\tGetting out of here...\n", Mix_GetError());
        exit(-1);
    }
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        LogMsgf(ERROR, "Failed to initialise SDL_Mixer. MIX_ERROR: %s\n\tGetting out of here...\n", Mix_GetError());
        exit(-1);
    }

    LogMsg(INFO, "SDL2 initialised successfully\n");}

void I_InitTextures()
{
    if(gTextures)
    {
        LogMsg(WARN, "Textures already initialised! Skipping...\n");
        return;
    }

    gTextures = malloc(sizeof(texture_t) * (NUMTEXTURES + 1));
    if(!gTextures)
    {
        LogMsg(ERROR, "Failed to allocate memory for textures array!\n");
        exit(-1);
    }

    for(int i = 0; i < NUMTEXTURES; i++)
    {
        gTextures[i] = T_LoadTexture(gTexturePaths[i]);
        if(gTextures[i].data == NULL)
        {
            LogMsgf(ERROR, "Failed to load texture at path '%s'. IMG_ERROR: %s\n", gTexturePaths[i], IMG_GetError());
        }
    }

    // create a blank texture for the minimap
    gTextures[NUMTEXTURES] = T_CreateBlankTexture("MINIMAP", 256, 256);
    if(gTextures[NUMTEXTURES].data == NULL)
    {
        LogMsgf(ERROR, "Failed to create blank minimap texture. IMG_ERROR: %s\n", IMG_GetError());
    }
    NUMTEXTURES++; // increase the number of textures to account for the minimap texture
    LogMsg(INFO, "Textures initialised successfully\n");
}

void I_InitGraphics()
{
    if(!W_InitWindow(&gMainWindow, "RayCaster", 1280, 720))
    {
        LogMsg(ERROR, "FATAL: failed to create main window\n");
        exit(-1);
    }
    LogMsg(INFO, "Graphics initialised successfully\n");
}

void I_InitPlayer()
{
    gPlayer.pos         = V_Make(4.0f, 4.0f);
    gPlayer.viewAngle   = 0.0f;
    gPlayer.maxMoveSpeed= MOVESPEED;
    gPlayer.rotateSpeed = ROTATESPEED;
    gPlayer.footstepSoundCooldown = 500.f;
}

void I_InitAudio()
{
    gFootstep1 = Mix_LoadWAV("res/sound/footsteps/1.wav");
    if(!gFootstep1)
    {
        LogMsgf(ERROR, "Failed to load footstep1 sound. MIX_ERROR: %s\n", Mix_GetError());
        return;
    }

    gFootstep2 = Mix_LoadWAV("res/sound/footsteps/2.wav");
    if(!gFootstep2)
    {
        LogMsgf(ERROR, "Failed to load footstep2 sound. MIX_ERROR: %s\n", Mix_GetError());
        Mix_FreeChunk(gFootstep1);
        gFootstep1 = NULL;
        return;
    }

    LogMsg(INFO, "Audio initialised successfully\n"); 
}

void I_CleanUp()
{
    M_Free(gMap);

    if(gTextures)
    {
        for(int i = 0; i < NUMTEXTURES; i++)
            T_FreeTexture(&gTextures[i]);
        free(gTextures);
    }

    if(gFootstep1)
        Mix_FreeChunk(gFootstep1);
    if(gFootstep2)
        Mix_FreeChunk(gFootstep2);

    FreeLogs();

    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}