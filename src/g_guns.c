#include "g_guns.h"

#include "logger.h"

void GUN_Update(gun_t* gun, float dtMs)
{
    if(!gun)
    {
        LogMsg(WARN, "passed null ptr to gun");
        return;
    }

    gun->cooldown += dtMs;

    T_UpdateAnimatedTexture(&gun->gunTexture, dtMs);

    if(gun->cooldown >= gun->fireRate)
    {
        gun->cooldown = gun->fireRate;
        gun->canShoot = true;
    }
    else
    {
        gun->canShoot = false;
    }
}