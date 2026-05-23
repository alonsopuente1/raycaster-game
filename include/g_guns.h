#ifndef __G_GUNS_H__
#define __G_GUNS_H__

#include "texture.h"

/* g_guns.h

    contains enum for the different guns the player can have

*/

typedef struct weaponAnim_s
{
    int         currentFrame;
    int         totalFrames;
    float       frameDuration;
    float       elapsedTime;
    bool        isShooting;
} weaponAnim_t;

typedef struct gun_s
{
    texture_t*      gunTexture;
    weaponAnim_t    anim;

    // firerate in ms
    int             fireRate;
    
    // cooldown until can next shoot
    int             cooldown;

    // scaling value for the texture
    float           texScale;
} gun_t;

#endif