#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "v_vert.h"

typedef struct player_s
{
    vertex2d_t      pos;
    float           viewAngle;
    float           moveSpeed;
    float           rotateSpeed;
    char            moveState;  // first bit = forward, second bit = backward, third bit = left, fourth bit = right, fifth bit = rotate right, sixth bit = rotate left
} player_t;

#endif