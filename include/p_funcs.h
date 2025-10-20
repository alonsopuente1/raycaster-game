#ifndef __P_FUNCS_H__
#define __P_FUNCS_H__

#include "p_player.h"

typedef struct map_s map_t;

// positive moves forward, negative backwards
extern void P_Move(player_t* p, const float distance);
// positive distance moves player left, negative moves player right
extern void P_Strafe(player_t* p, const float distance);
extern void P_Rotate(player_t* p, const float ang);
// if map pointer passed, handle collision.
// dt is passed in milliseconds
extern void P_HandleState(player_t* p, map_t* m, float dt);       

#endif