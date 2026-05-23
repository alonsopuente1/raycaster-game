#include "e_entitymanager.h"

#include "logger.h"
#include "map.h"
#include "v_funcs.h"

entity_t* EM_PushEntity(entitymanager_t* em, entity_t* e)
{
    if(!em)
    {
        LogMsg(ERROR, "passed null ptr to entitymanager\n");
        return NULL;
    }

    if(!e)
    {
        LogMsg(ERROR, "passed null ptr to entity");
        return NULL;
    }

    if(em->numEntities > EM_MAX_ENTITIES)
    {
        LogMsg(WARN, "reached max number of entities!\n");
        return NULL;
    }

    em->entities[em->numEntities] = *e;

    e->currentEM = em;

    return &em->entities[em->numEntities++];
}

void EM_UpdateEntities(entitymanager_t* em, float deltaTime, map_t* m)
{
    for(unsigned int i = 0; i < em->numEntities; i++)
    {
        E_Update(&em->entities[i], deltaTime);
        if(em->entities[i].markForDeletion)
        {
            EM_RemoveEntity(em, &em->entities[i]);
            i--;
        }
    }
}

entity_t* EM_IsInEntityList(entitymanager_t* em, entity_t* e)
{
    if(!em)
    {
        LogMsg(ERROR, "passed null ptr to entity manager\n");
        return NULL;
    }

    if(!e)
    {
        LogMsg(ERROR, "passed null ptr to entity\n");
        return NULL;
    }

    // first check if e is in the address space of em->entities 
    if(em->entities <= e && e <= em->entities + em->numEntities - 1)
        return e;

    // otherwise check if its a copy
    for(unsigned int i = 0; i < em->numEntities; i++)
        if(E_IsEqual(em->entities[i], *e))
            return &em->entities[i];

    return NULL;
}

bool EM_RemoveEntity(entitymanager_t* em, entity_t* e)
{
    if(!em)
    {
        LogMsg(ERROR, "passed null ptr to entity manager\n");
        return false;
    }

    if(!e)
    {
        LogMsg(ERROR, "passed null ptr to entity\n");
        return false;
    }

    if(!em->numEntities)
    {
        LogMsg(DEBUG, "no entities in entity list to remove\n");
        return false;
    }

    entity_t* entity = EM_IsInEntityList(em, e);

    if(!entity)
    {
        LogMsg(WARN, "entity is not in entity list\n");
        return false;
    }

    // if entity is last one in the list simple remove operation, pop it off
    if(entity == &em->entities[em->numEntities - 1])
    {
        em->entities[em->numEntities - 1] = (entity_t){0};
    }
    // else every entity in front needs to be moved one space back
    else
        memmove(entity, entity + 1, (int)sizeof(entity_t) * (int)(&em->entities[em->numEntities - 1] - entity));
    
    em->numEntities--;
    return true;
}

void EM_InitEntityManager(entitymanager_t* em)
{
    if(!em)
    {
        LogMsg(ERROR, "passed null ptr to entity manager\n");
        return;
    }

    em->numEntities = 0;
    memset(em->entities, 0, sizeof(entity_t) * EM_MAX_ENTITIES);

    for(int i = 0; i < EM_MAX_ENTITIES; i++)
    {
        em->entities[i].active = false;
        em->entities[i].currentEM = em;
    }
}

entity_t* EM_Raycast(entitymanager_t* em, vertex2d_t origin, vertex2d_t dir, map_t* map, float* outDist)
{
    if(!em)
        return NULL;

    vertex2d_t normDir = V_Normalise(dir);

    // find distance to wall first
    vertex2d_t wallCollision = {0.f, 0.f};
    RayHitDesc side = RAY_HIT_NONE;
    float wallDist = INFINITY;
    if(M_RayCollision(map, origin, dir, &side, &wallCollision))
    {
        wallDist = V_GetMagnitude(V_Sub(wallCollision, origin));
    }

    float bestT = INFINITY;
    entity_t* best = NULL;

    // simple radius-based hit test: if perpendicular distance from ray to entity <= hitRadius
    const float hitRadius = 0.6f;

    for(unsigned int i = 0; i < em->numEntities; i++)
    {
        entity_t* e = &em->entities[i];
        if(!e->active)
            continue;

        vertex2d_t toEnt = V_Sub(e->pos, origin);
        float t = V_DotProduct(toEnt, normDir);
        if(t <= 0.f)
            continue; // behind origin

        vertex2d_t closest = V_Add(origin, V_Mul(normDir, t));
        float perpDist = V_GetMagnitude(V_Sub(closest, e->pos));

        if(perpDist <= hitRadius && t < wallDist && t < bestT)
        {
            bestT = t;
            best = e;
        }
    }

    if(outDist)
        *outDist = best ? bestT : INFINITY;

    return best;
}