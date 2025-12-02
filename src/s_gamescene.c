#include "s_gamescene.h"

#include "s_gameevents.h"
#include "r_draw.h"
#include "p_funcs.h"
#include "v_funcs.h"
#include "m_game.h"
#include "w_window.h"

#include "logger.h"

#include <SDL2/SDL.h>

/* FORWARD DECLARATIONS */

void GS_HandleUserEvent(void* scene, maingame_t* game, SDL_Event* e);

/* PUBLIC FUNCTIONS*/

void GS_SetupScene(void* scene, maingame_t* game)
{
    gamescene_t* gScene = (gamescene_t*)scene;

    memset(gScene, 0, sizeof(gamescene_t));

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
        "res/textures/guns/FIST.png",
        "res/textures/enemies/cacodemon.png"
    };
    
    gScene->renderer = R_CreateRenderer(&game->window);
    if(!gScene->renderer.parentWindow)
    {
        LogMsg(ERROR, "failed to create renderer for game scene");
        G_ChangeScene(game, "MainMenu");
        return;
    }
    
    /* TEXTURE LOADING */
    int NUMTEXTURES = ((int)(sizeof(texturePaths) / sizeof(texturePaths[0])));
    
    for(int i = 0; i < NUMTEXTURES; i++)
    {
        if(!TB_PushTexture(gScene->renderer.parentWindow, &gScene->renderer.textureBank, texturePaths[i]))
        {
            LogMsgf(ERROR, "failed to load texture at file path '%s'\n", texturePaths[i]);
        }
    }    
    
    gScene->entity = (entity_t){
        TB_FindTextureByName(&gScene->renderer.textureBank, "cacodemon"),
        (vertex2d_t){0.f, 0.f},
        (vertex2d_t){0.f, 0.f},
        (vertex2d_t){3.f, 3.f},
    };

    texture_t* texture = TB_AddEmptyTexture(&gScene->renderer.textureBank);
    if(!texture)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", "Failed to add empty texture to texture bank for minimap!", NULL);
        G_ChangeScene(game, "MainMenu");
        return;
    }

    *texture = T_CreateBlankTexture(gScene->renderer.parentWindow, "MINIMAP", 256, 256);
    if(!texture->data)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", "Failed to add empty texture for minimap!", NULL);
        G_ChangeScene(game, "MainMenu");
        return;
    }

    /* AUDIO LOADING */

    gScene->footstep1 = Mix_LoadWAV("res/sound/footsteps/1.wav");
    if(!gScene->footstep1)
    {
        char msgBuffer[256] = {0};
        snprintf(msgBuffer, sizeof(msgBuffer), "Failed to load footstep sound! MIX_ERROR: %s", Mix_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", msgBuffer, NULL);
        G_ChangeScene(game, "MainMenu");
        return;
    }

    gScene->footstep2 = NULL;

    gScene->footstep2 = Mix_LoadWAV("res/sound/footsteps/2.wav");
    if(!gScene->footstep2)
    {
        char msgBuffer[256] = {0};
        snprintf(msgBuffer, sizeof(msgBuffer), "Failed to load footstep sound! MIX_ERROR: %s", Mix_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", msgBuffer, NULL);
        G_ChangeScene(game, "MainMenu");
        return;
    }
}

void GS_HandleEvents(void* scene, maingame_t* game, SDL_Event* e)
{
    gamescene_t* gScene = (gamescene_t*)scene;

    if(!e)
        return;

    player_t* player = &gScene->player;
    
    if(e->type == SDL_MOUSEMOTION)
        P_Rotate(player, (float)(e->motion.xrel) / 100);

    if(e->type == SDL_USEREVENT)
        GS_HandleUserEvent(scene, game, e);
    
    
    // will never return null since sdl is initialised
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    
    if(keys[SDL_SCANCODE_ESCAPE])
    {
        G_ChangeScene(game, "MainMenu");
        return;
    }

    if(keys[SDL_SCANCODE_W])
        player->moveState |= MOVE_FORWARD;
    else
        player->moveState &= ~MOVE_FORWARD;

    if(keys[SDL_SCANCODE_S])
        player->moveState |= MOVE_BACKWARD;
    else
        player->moveState &= ~MOVE_BACKWARD;

    if(keys[SDL_SCANCODE_A])
        player->moveState |= STRAFE_LEFT;
    else
        player->moveState &= ~STRAFE_LEFT;

    if(keys[SDL_SCANCODE_D])
        player->moveState |= STRAFE_RIGHT;
    else
        player->moveState &= ~STRAFE_RIGHT;

    if(keys[SDL_SCANCODE_RIGHT])
        player->moveState |= ROTATE_RIGHT;
    else
        player->moveState &= ~ROTATE_RIGHT;
        
    if(keys[SDL_SCANCODE_LEFT])
        player->moveState |= ROTATE_LEFT;
    else
        player->moveState &= ~ROTATE_LEFT;   
}

void GS_Update(void* scene, maingame_t* game, float dt)
{
    gamescene_t* gScene = (gamescene_t*)scene;

    // game is not ready yet since there is no map yet
    if(!gScene->map.mapData)
        return;

    P_HandleState(&gScene->player, &gScene->map, dt);

    if(SDL_GetWindowFlags(game->window.sdlWindow) & SDL_WINDOW_INPUT_FOCUS)
        SDL_SetRelativeMouseMode(SDL_TRUE);
    else
        SDL_SetRelativeMouseMode(SDL_FALSE);

    if(gScene->player.footstepSoundCooldown > -1000.f)
        gScene->player.footstepSoundCooldown -= dt;

    bool sound = false;
    if(V_GetMagnitude(gScene->player.vel) > 0.00007f && gScene->player.footstepSoundCooldown < 0)
    {
        gScene->player.footstepSoundCooldown = 500.f;
        sound ? Mix_PlayChannel(-1, gScene->footstep1, 0) : Mix_PlayChannel(-1, gScene->footstep2, 0);
        sound = !sound;
    }
}

void GS_Draw(void* scene, maingame_t* game)
{
    gamescene_t* gScene = (gamescene_t*)scene;
    renderer_t* render = &gScene->renderer;

    if(!gScene->map.mapData)
        return;

    R_ClearScreen(render, (SDL_Color){0, 0, 0, 255});

    R_RenderCeilingAndFloor(render, (SDL_Color){40, 40, 40, 255}, (SDL_Color){60, 60, 60, 255});
    R_RenderPlayerView(render, &gScene->player, &gScene->map);
    R_RenderPlayerGun(render, &gScene->player);
    R_RenderMinimap(render, &gScene->player, &gScene->map);
    E_DrawEntity(render, &gScene->player, &gScene->entity);

    R_Present(render);
}

void GS_DestroyScene(void* scene, maingame_t* game)
{
    gamescene_t* gScene = (gamescene_t*)scene;

    R_DestroyRenderer(&gScene->renderer);

    if(gScene->footstep1)
        Mix_FreeChunk(gScene->footstep1);
    if(gScene->footstep2)
        Mix_FreeChunk(gScene->footstep2);

    M_Free(&gScene->map);
}

/* PRIVATE FUNCTIONS */
void GS_HandleUserEvent(void* scene, maingame_t* game, SDL_Event* e)
{
    if(!e || e->user.type != SDL_USEREVENT)
        return;

    gamescene_t* gScene = (gamescene_t*)scene;

    switch(e->user.code)
    {
    case EVENT_LOADMAP:
    {
        maploadargs_t mapArgs = { 0 };
        M_LoadMap(&gScene->map, &mapArgs, e->user.data1);
        if(e->user.data1)
            free(e->user.data1);
        
        if(!mapArgs.success)
        {
            LogMsgf(ERROR, "failed to load map at file path '%s'\n", (char*)e->user.data1);
            G_ChangeScene(game, "MainMenu");
            return;
        }

        gScene->player.maxMoveSpeed = mapArgs.maxSpeed;
        gScene->player.pos = mapArgs.startPos;
        gScene->player.rotateSpeed = mapArgs.rotateSpeed;
        gScene->player.fov = 90.f * (M_PI / 180.f);
        
        break;
    } // EVENT_LOADMAP

    } // switch
    
}