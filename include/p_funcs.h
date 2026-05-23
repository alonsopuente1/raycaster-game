#ifndef __P_FUNCS_H__
#define __P_FUNCS_H__

#include "p_player.h"

/* p_funcs.h

    contains functions for handling player state
    
*/

typedef struct map_s map_t;
typedef struct entitymanager_s entitymanager_t;

// positive moves forward, negative backwards
extern void P_Move(player_t* p, const float distance);
// positive distance moves player left, negative moves player right
extern void P_Strafe(player_t* p, const float distance);
extern void P_Rotate(player_t* p, const float ang);
// if map pointer passed, handle collision.
// dt is passed in milliseconds
extern void P_HandleState(player_t* p, map_t* m, float dt);       

// handle player shooting (raycast). checks gun cooldown and removes hit entity.
extern void P_Shoot(player_t* p, map_t* m, entitymanager_t* em);

#endif