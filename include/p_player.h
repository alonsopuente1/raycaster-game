#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "v_vert.h"
#include "w_guns.h"

typedef enum movestateBitmask_e
{
    MOVE_FORWARD        = 1 << 0,
    MOVE_BACKWARD       = 1 << 1,
    STRAFE_LEFT         = 1 << 2,
    STRAFE_RIGHT        = 1 << 3,
    ROTATE_RIGHT        = 1 << 4,
    ROTATE_LEFT         = 1 << 5
} movestateBitmask_t;

typedef struct player_s
{
    float           viewAngle;
    float           maxMoveSpeed;
    
    vertex2d_t      acc;
    vertex2d_t      vel;
    vertex2d_t      pos;

    float           rotateSpeed;
    char            moveState;  // first bit = forward, second bit = backward, third bit = left, fourth bit = right, fifth bit = rotate right, sixth bit = rotate left
    
    gun_t           currentGun; 
    float           gunSway;

} player_t;

#endif