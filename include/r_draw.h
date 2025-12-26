#ifndef __R_DRAW_H__
#define __R_DRAW_H__

#include <SDL2/SDL.h>
#include <stdbool.h>



typedef struct renderer_s renderer_t;
typedef struct player_s player_t;
typedef struct map_s map_t;
typedef struct entitymanager_s entitymanager_t;
typedef struct texture_s texture_t;

extern void R_RenderPlayerView(renderer_t* render, player_t* player, map_t* map);
extern void R_RenderCeilingAndFloor(renderer_t* render, SDL_Color topColour, SDL_Color bottomColour);
extern void R_RenderPlayerGun(renderer_t* render, player_t* player);
// tex must be in the texturebank of the renderer for this to work
extern bool R_RenderTexture(renderer_t* render, texture_t* tex, SDL_Rect src, SDL_Rect dst);

// requires a texture called MINIMAP in the texturebank of renderer
extern void R_RenderMinimap(renderer_t* render, player_t* player, entitymanager_t* em, map_t* map);
extern texture_t* R_UpdateMinimap(renderer_t* render, player_t* player, entitymanager_t* em, map_t* map);
extern bool R_DebugMinimap(renderer_t* render, map_t* map, player_t* player, entitymanager_t* em);

#endif