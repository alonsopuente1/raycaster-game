#ifndef __S_GAMESCENE_H__
#define __S_GAMESCENE_H__

#include "s_scene.h"

#include "p_player.h"
#include "map.h"
#include "r_renderer.h"
#include "e_entity.h"
#include "e_entitymanager.h"

#include <SDL2/SDL_mixer.h>

/* s_gamescene.h

    this scene is the main gameplay scene

*/

#pragma pack(push, 1)
typedef struct gamescene_s
{

    SetupFunc           setup;
    HandleEventsFunc    handleEvents;
    UpdateFunc          update;
    DrawFunc            draw;
    DestroyFunc         destroy;

    player_t            player;
    map_t               map;

    Mix_Chunk*          footstep1;
    Mix_Chunk*          footstep2;  

    entitymanager_t     entityManager;

    bool                debugMinimapToggle;

    renderer_t          renderer;
} gamescene_t;
#pragma pack(pop)

extern void GS_SetupScene(void* scene, maingame_t* game);
extern void GS_HandleEvents(void* scene, maingame_t* game, SDL_Event* e);
extern void GS_Update(void* scene, maingame_t* game, float dt);
extern void GS_Draw(void* scene, maingame_t* game);
extern void GS_DestroyScene(void* scene, maingame_t* game);

#endif
