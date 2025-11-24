#include "e_entitymanager.h"

#include "logger.h"

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

    return &em->entities[em->numEntities++];
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

    // if e is in the address space of em->entities 
    if(em->entities <= e && e <= em->entities + em->numEntities - 1)
        return e;

    // else check if its a copy
    for(int i = 0; i < em->numEntities; i++)
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

    if(e == &em->entities[em->numEntities - 1])
    {
        
    }

    return false;
}
