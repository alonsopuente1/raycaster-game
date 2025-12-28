#ifndef __S_MAINMENUSCENE_H__
#define __S_MAINMENUSCENE_H__

#include "s_scene.h"
#include "w_buttons.h"
#include "r_renderer.h"

/* s_mainmenuscene.h

    this scene is the main menu scene

*/

typedef enum menustate_e
{
    MAINMENU,
    MAPCHOOSE
} menustate_t;

#pragma pack(push, 1)
typedef struct mainMenuScene_s
{
    SetupFunc           setup;
    HandleEventsFunc    handleEvents;
    UpdateFunc          update;
    DrawFunc            draw; 
    DestroyFunc         destroy;

    renderer_t          render;

    menustate_t         state;

    // mainmenu elements
    winButton_t         startButton;
    winButton_t         exitButton;

    // mapchoose elements
    winButton_t*        mapFileButtons;

    winButton_t         backButton;

    // misc data
    int                 numMapFiles;
    char**              mapFiles;
} mainMenuScene_t;
#pragma pack(pop)

extern void MMS_SetupScene(void* scene, maingame_t* game);
extern void MMS_HandleEvents(void* scene, maingame_t* game, SDL_Event* event);
extern void MMS_Update(void* scene, maingame_t* game, float dt);
extern void MMS_Draw(void* scene, maingame_t* game);
extern void MMS_Destroy(void* scene, maingame_t* game);

#endif