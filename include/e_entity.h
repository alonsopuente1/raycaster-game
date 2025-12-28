#ifndef __E_ENTITY_H__
#define __E_ENTITY_H__

#include "v_vert.h"

#include <stdbool.h>

/* e_entity.h

    contains the definition for the entity_t struct
    this struct is the "base" struct for all entities 
    except player in game

    entity behaviour should be implemented using "think" functions

    the think function should only SET the STATE of the entity for it to be update 
    in other words, position should not be altered. instead,
    the velocity/acceleration should be set.

    in E_Update, the think function is called then 

*/

typedef struct texture_s texture_t;
typedef struct renderer_s renderer_t;
typedef struct player_s player_t;
typedef struct entitymanager_s entitymanager_t;
typedef struct map_s map_t;

typedef struct entity_s
{

    texture_t*          entityTex;

    vertex2d_t          acc;
    vertex2d_t          vel;
    vertex2d_t          pos;

    float               angle;
    float               maxSpeed;

    // if false, entity is not processed or drawn
    bool                active;

    // function pointer for entity specific logic
    void                (*think)(struct entity_s* e, float deltaTime);

    map_t*              currentMap;
    entitymanager_t*    currentEM;

    // this is only used for the entity manager to mark entities for deletion
    bool                markForDeletion;

} entity_t;

extern void E_DrawEntity(renderer_t* render, player_t* player, entity_t* e);
extern bool E_IsEqual(entity_t a, entity_t b);
// updates entity position and state
extern void E_Update(entity_t* e, float deltaTime);

#endif