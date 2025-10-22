#ifndef __M_GAME_H__
#define __M_GAME_H__

#include "w_window.h"
#include "w_buttons.h"
#include "p_player.h"
#include "texture.h"
#include "map.h"
#include <SDL2/SDL_mixer.h>
#include "texturebank.h"

#include "m_gamestate.h"

// this contains the main game data and game loop

typedef struct maingame_s
{
    window_t        window;
    player_t        player;
    map_t           map;

    maingamestate_t gameState;

    // will store the file path to the res folder with all game assets
    char            resFolderFilePath[32];

    bool            running;

    Mix_Chunk*      footstepSound1;
    Mix_Chunk*      footstepSound2;

    // pointer to dynamic array of buttons on screen
    winButton_t*    screenButtons;
    int             numButtons;

    texturebank_t   texturebank;
} maingame_t;


extern void G_Run(maingame_t* game);

extern void G_Init(maingame_t* game);

extern void G_HandleEvents(maingame_t* game);
// dt in milliseconds
extern void G_Update(maingame_t* game, float dt);
extern void G_Draw(maingame_t* game);

extern void G_Destroy(maingame_t* game);

#endif