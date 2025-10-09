#include "i_init.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "settings.h"
#include "p_player.h"
#include "v_funcs.h"
#include "m_map.h"
#include "logger.h"
#include <stdio.h>

extern int gScreenWidth;
extern int gScreenHeight;
extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern player_t gPlayer;
extern map_t gMap;
extern SDL_Texture* playerTex;
extern SDL_Texture* gTextures[NUMTEXTURES];

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
    for(int i = 0; i < NUMTEXTURES; i++)
    {
        gTextures[i] = IMG_LoadTexture(gRenderer, gTexturePaths[i]);
        if(!gTextures[i])
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
        printf("Failed to create window. SDL_ERROR: %s\n", SDL_GetError());
    }

	gRenderer = SDL_CreateRenderer(gWindow, -1, 0);
    if(!gRenderer)
    {
        printf("Failed to create renderer. SDL_ERROR: %s\n", SDL_GetError());
    }
}

void I_InitPlayer()
{
    gPlayer.pos         = V_Make(4.0f, 4.0f);
    gPlayer.viewAngle   = 0.0f;
    gPlayer.moveSpeed   = MOVESPEED;
    gPlayer.rotateSpeed = ROTATESPEED;
}

void I_CleanUp()
{
    M_Free(gMap);

    for(int i = 0; i < NUMTEXTURES; i++)
    {
        SDL_DestroyTexture(gTextures[i]);
    }

    FreeLogs();

    SDL_DestroyTexture(playerTex);

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    IMG_Quit();
    SDL_Quit();
}