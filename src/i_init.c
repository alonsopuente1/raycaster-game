#include "i_init.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "settings.h"
#include "p_player.h"
#include "v_funcs.h"
#include "m_map.h"
#include "logger.h"
#include <stdio.h>
#include "t_textures.h"

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
    "res/textures/guns/PIST2.png"
};

int NUMTEXTURES = ((int)(sizeof(gTexturePaths) / sizeof(gTexturePaths[0])));

extern int gScreenWidth;
extern int gScreenHeight;
extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern player_t gPlayer;
extern map_t gMap;
extern SDL_Texture* playerTex;

void I_InitLibs()
{
    if(!IMG_Init(IMG_INIT_PNG))
    {
        LogMsgf(ERROR, "Failed to initialise SDL_image. Error: %s Getting out of here...", IMG_GetError());
        exit(-1);
    }
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        LogMsgf(ERROR, "Failed to init SDL2. SDL_ERROR: %s\n Getting out of here...", SDL_GetError());
        exit(-1);
    }
}

void I_InitTextures()
{
    if(gTextures)
    {
        LogMsg(WARN, "Textures already initialised! Skipping...");
        return;
    }

    gTextures = malloc(sizeof(texture_t) * NUMTEXTURES);
    if(!gTextures)
    {
        LogMsg(ERROR, "Failed to allocate memory for textures array!");
        exit(-1);
    }

    for(int i = 0; i < NUMTEXTURES; i++)
    {
        gTextures[i] = T_LoadTexture(gTexturePaths[i]);
        if(gTextures[i].data == NULL)
        {
            LogMsgf(ERROR, "Failed to load texture at path '%s'. IMG_ERROR: %s", gTexturePaths[i], IMG_GetError());
        }
    }
}

void I_InitGraphics()
{
    gWindow	= SDL_CreateWindow("RayCaster", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gScreenWidth, gScreenHeight, 0);
    if(!gWindow)
    {
        LogMsgf(ERROR, "Failed to create window. SDL_ERROR: %s\n", SDL_GetError());
    }

	gRenderer = SDL_CreateRenderer(gWindow, -1, 0);
    if(!gRenderer)
    {
        LogMsgf(ERROR, "Failed to create renderer. SDL_ERROR: %s\n", SDL_GetError());
    }
}

void I_InitPlayer()
{
    gPlayer.pos         = V_Make(4.0f, 4.0f);
    gPlayer.viewAngle   = 0.0f;
    gPlayer.maxMoveSpeed= MOVESPEED;
    gPlayer.rotateSpeed = ROTATESPEED;
}

void I_CleanUp()
{
    M_Free(gMap);

    for(int i = 0; i < NUMTEXTURES; i++)
    {
        T_FreeTexture(&gTextures[i]);
    }

    if(gTextures)
        free(gTextures);

    FreeLogs();

    SDL_DestroyTexture(playerTex);

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    IMG_Quit();
    SDL_Quit();
}