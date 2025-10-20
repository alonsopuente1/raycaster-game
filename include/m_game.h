#ifndef __M_GAME_H__
#define __M_GAME_H__

#include "window.h"
#include "p_player.h"
#include "texture.h"
#include "map.h"
#include <SDL2/SDL_mixer.h>
#include "texturebank.h"

// this contains the main game data and game loop

typedef struct maingame_s
{
    window_t        window;
    player_t        player;
    map_t           map;

    // will store the file path to the res folder with all game assets
    char            resFolderFilePath[32];

    bool            running;

    Mix_Chunk*      footstepSound1;
    Mix_Chunk*      footstepSound2;

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