#ifndef __R_DRAW_H__
#define __R_DRAW_H__

#include "m_game.h"

typedef struct texturebank_s texturebank_t;

extern void R_RenderPlayerView(maingame_t* game);
extern void R_RenderCeilingAndFloor(maingame_t* game);
extern void R_RenderPlayerGun(maingame_t* game);

// requires a texture called MINIMAP in the texturebank of game
extern void R_RenderMinimap(maingame_t* game);

#endif