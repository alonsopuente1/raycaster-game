#ifndef __G_GUNS_H__
#define __G_GUNS_H__

#include "animatedtexture.h"

/* g_guns.h

    contains enum for the different guns the player can have

*/

typedef struct gun_s
{
    animatedTexture_t   gunTexture;

    // firerate in ms
    int                 fireRate;
    
    // cooldown in ms 
    int                 cooldown;

    // scaling value for the texture
    float               texScale;

    bool                canShoot;
} gun_t;

extern void GUN_Update(gun_t* gun, float dtMs);

#endif