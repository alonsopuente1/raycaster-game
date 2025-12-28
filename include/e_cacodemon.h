#ifndef __E_CACODEMON_H__
#define __E_CACODEMON_H__

/* e_cacodemon.h

    contains the create function and thinker functions for the enemy
    cacodemon

*/

typedef struct entity_s entity_t;

// creates and returns a cacodemon entity at the given position
// entityTex, currentMap, and currentEM are set to NULL and must be set manually
extern entity_t CACOD_CreateCacodemonEntity(float x, float y);

extern void CACOD_Walk(entity_t* e, float deltaTime);

#endif