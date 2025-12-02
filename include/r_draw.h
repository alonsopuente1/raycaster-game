#ifndef __R_DRAW_H__
#define __R_DRAW_H__

#include <SDL2/SDL.h>

#include "r_renderer.h"

typedef struct player_s player_t;
typedef struct map_s map_t;
typedef struct entitymanager_s entitymanager_t;

extern void R_RenderPlayerView(renderer_t* render, player_t* player, map_t* map);
extern void R_RenderCeilingAndFloor(renderer_t* render, SDL_Color topColour, SDL_Color bottomColour);
extern void R_RenderPlayerGun(renderer_t* render, player_t* player);

// requires a texture called MINIMAP in the texturebank of renderer
extern void R_RenderMinimap(renderer_t* render, player_t* player, entitymanager_t* em, map_t* map);

#endif