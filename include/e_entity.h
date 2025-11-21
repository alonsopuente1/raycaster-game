#ifndef __E_ENTITY_H__
#define __E_ENTITY_H__

#include "v_vert.h"

typedef struct texture_s texture_t;
typedef struct renderer_s renderer_t;
typedef struct player_s player_t;

typedef struct entity_s
{

    texture_t*      entityTex;

    vertex2d_t      acc;
    vertex2d_t      vel;
    vertex2d_t      pos;

} entity_t;

extern void E_DrawEntity(renderer_t* render, player_t* player, entity_t* e);

#endif