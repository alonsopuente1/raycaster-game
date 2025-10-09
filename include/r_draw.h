#ifndef __R_DRAW_H__
#define __R_DRAW_H__

#include "p_player.h"
#include "m_map.h"

extern void R_RenderPlayerView(player_t* p, map_t* map);
extern void R_RenderMap(player_t* p, map_t* map);
extern void R_RenderCeilingAndFloor(void);

#endif