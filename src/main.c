#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

#include "p_player.h"
#include "p_funcs.h"
#include "v_vert.h"
#include "v_funcs.h"
#include "r_draw.h"
#include "m_map.h"
#include "t_textures.h"
#include "settings.h"
#include "i_init.h"
#include "logger.h"

int gScreenWidth     = 1280;
int gScreenHeight    = 720;

SDL_Window*     gWindow;
SDL_Renderer*   gRenderer;

map_t gMap;

player_t gPlayer;

SDL_Texture* playerTex;

bool shouldClose = false;

void E_HandleEvents(float dt)
{
    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
        switch(e.type)
        {
        case SDL_QUIT:
            shouldClose = true;
            return;
        }
    }
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    
    if(keys[SDL_SCANCODE_ESCAPE])
    {
        shouldClose = true;
        return;
    }

    if(keys[SDL_SCANCODE_W])
        gPlayer.moveState |= 1;
    else
        gPlayer.moveState &= ~1;

    if(keys[SDL_SCANCODE_S])
        gPlayer.moveState |= 1 << 1;
    else
        gPlayer.moveState &= ~(1 << 1);

    if(keys[SDL_SCANCODE_A])
        gPlayer.moveState |= 1 << 2;
    else
        gPlayer.moveState &= ~(1 << 2);

    if(keys[SDL_SCANCODE_D])
        gPlayer.moveState |= 1 << 3;
    else
        gPlayer.moveState &= ~(1 << 3);

    if(keys[SDL_SCANCODE_RIGHT])
        gPlayer.moveState |= 1 << 4;
    else
        gPlayer.moveState &= ~(1 << 4);
        
    if(keys[SDL_SCANCODE_LEFT])
        gPlayer.moveState |= 1 << 5;
    else
        gPlayer.moveState &= ~(1 << 5);
    
    Uint32 flags = SDL_GetWindowFlags(gWindow);

    if(flags & SDL_WINDOW_INPUT_FOCUS)
    {
        int x, _;
        SDL_GetMouseState(&x, &_);

        int deltaX = x - gScreenWidth / 2;

        P_Rotate(&gPlayer, deltaX * gPlayer.rotateSpeed * dt);
        SDL_WarpMouseInWindow(gWindow, gScreenWidth / 2, gScreenHeight / 2);

        SDL_ShowCursor(SDL_DISABLE);
    }
    else
    {
        SDL_ShowCursor(SDL_ENABLE);
    }
}

int main(int argc, char** argv)
{
    LogMsg(DEBUG, "Starting RayCaster...\n");

    if(argc < 2)
    {
        LogMsg(ERROR, "No map file specified\n\t\tUsage: RayCaster <map_file>\n");
        return -1;
    }

    // Initialise everything
    I_InitLibs();
    I_InitGraphics();
    I_InitTextures();
    I_InitPlayer();

    // Load map from file
    gMap = M_LoadMap(argv[1]);

    if(gMap.mapData == NULL)
    {
        LogMsg(ERROR, "Failed to load map\n"); 
        return -1;
    }

    float oldTime = (float)SDL_GetTicks();
    // printf("First Frame\n");
    while(!shouldClose)
    {
        SDL_Delay(1000.f / 60.f);
        float newTime = (float)SDL_GetTicks();
        float dt = newTime - oldTime;
        float fps = 1000.f / dt;
        oldTime = newTime;

        char newTitle[64];
        snprintf(newTitle, 64, "RayCaster %.0ffps", fps);
                
        SDL_SetWindowTitle(gWindow, newTitle);

        // Clear screen
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xff);
        SDL_RenderClear(gRenderer);

        // Main loop
        E_HandleEvents(dt);
        P_HandleState(&gPlayer, &gMap, dt);

        R_RenderCeilingAndFloor();
        R_RenderPlayerView(&gPlayer, &gMap);
        R_RenderPlayerGun(&gPlayer);

        // printf("PlayerPos: %.2f, %.2f\n", gPlayer.pos.x, gPlayer.pos.y);

        // Present
        SDL_RenderPresent(gRenderer);

    }

    I_CleanUp();

    return 0;
}