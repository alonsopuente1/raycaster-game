#ifndef __E_ENTITYMANAGER_H__
#define __E_ENTITYMANAGER_H__

#include "e_entity.h"

#include <stdbool.h>

#define EM_MAX_ENTITIES 128

typedef struct entitymanager_s
{

    entity_t        entities[EM_MAX_ENTITIES];
    unsigned int    numEntities;

} entitymanager_t;

// copys entity into the list
// returns pointer to the new entity in the array
extern entity_t* EM_PushEntity(entitymanager_t* em, entity_t* e);

// returns pointer to the entity in the list
extern entity_t* EM_IsInEntityList(entitymanager_t* em, entity_t* e);
// doesnt matter if entity is copy or belongs to the list
extern bool EM_RemoveEntity(entitymanager_t* em, entity_t* e);

#endif