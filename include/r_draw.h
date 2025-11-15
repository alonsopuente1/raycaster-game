#ifndef __R_DRAW_H__
#define __R_DRAW_H__

#include "m_game.h"
#include "s_gamescene.h"

extern void R_RenderPlayerView(gamescene_t* scene, maingame_t* game);
extern void R_RenderCeilingAndFloor(gamescene_t* scene, maingame_t* game);
extern void R_RenderPlayerGun(gamescene_t* scene, maingame_t* game);

// requires a texture called MINIMAP in the texturebank of game
extern void R_RenderMinimap(gamescene_t* scene, maingame_t* game);

#endif