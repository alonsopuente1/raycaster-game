#ifndef __R_DRAW_H__
#define __R_DRAW_H__

#include "p_player.h"
#include "map.h"
#include "window.h"

typedef struct texturebank_s texturebank_t;

extern void R_RenderPlayerView(window_t* window, texturebank_t* texturebank, player_t* p, map_t* map);
extern void R_RenderCeilingAndFloor(window_t* window);
extern void R_RenderPlayerGun(window_t* window, texturebank_t* texturebank, player_t* p);
extern void R_RenderMinimap(window_t* window, texturebank_t* texturebank, player_t* p, map_t* map);

#endif