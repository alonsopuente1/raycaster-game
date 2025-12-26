#include "e_cacodemon.h"

#include "e_entity.h"
#include "v_funcs.h"

#include "logger.h"

entity_t CACOD_CreateCacodemonEntity(float x, float y)
{
    entity_t e = {
        .entityTex = NULL,
        .acc = { 0.f, 0.f },
        .vel = { 0.f, 0.f },
        .pos = { x, y },
        .active = true,
        .think = CACOD_Walk,
        .currentMap = NULL,
        .currentEM = NULL,
        .maxSpeed = 0.0015f
    };

    return e;
}

void CACOD_Walk(entity_t* e, float deltaTime)
{
    if(!e)
    {
        LogMsg(WARN, "passed null ptr to cacodemon entity\n");
        return;
    }

    // simple AI: move forward in the direction it's facing
    vertex2d_t dir = {
        cosf(e->angle),
        sinf(e->angle)
    };

    V_SetMagnitude(&dir, 0.001f);

    e->acc = dir;
}

void CACOD_Attack(entity_t* e, float deltaTime)
{
    if(!e)
    {
        LogMsg(ERROR, "passed null ptr to entity\n");
        return;
    }

    

    return; 
}