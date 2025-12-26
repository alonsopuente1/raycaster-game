#ifndef __MAP_H__
#define __MAP_H__

#include "v_vert.h"

#include <stdbool.h>

/* map.h

    contains definition and functions for struct map_t which 
    only stores a 2d array and file path that it was loaded from

*/

// stores extra information of map loading
typedef struct maploadargs_s
{
    // stores start position of player in map
    vertex2d_t  startPos;

    float       maxSpeed;
    float       rotateSpeed;

    // true when map loaded successfully
    bool        success;

} maploadargs_t;

typedef struct map_s
{
    // flat 2d array
    // dynamically allocated
    int*    mapData;

    int     mapWidth;
    int     mapHeight;

    // stores the filepath of the map on succesful load
    // dynamically allocated
    char*   filePath;
} map_t;

// returns -1 on out of range value, passing NULL to map or passing index out of range of map
extern int M_GetMapCell(map_t* map, int index);

// if mapArgs->success == false then map failed to load
// error msg set. use GetError() to get... the error
extern void M_LoadMap(map_t* map, maploadargs_t* mapArgs, const char* filePath);

// RAY_HIT_VERTICAL: means a ray hit a vertical wall, i.e., the ray crossed a vertical line on the grid
// RAY_HIT_HORIZONTAL: means a ray hit a horizontal wall, i.e., the ray crossed a horizontal line on the grid
typedef enum 
{ 
    RAY_HIT_NONE = -1, 
    RAY_HIT_HORIZONTAL = 0, 
    RAY_HIT_VERTICAL = 1 
} RayHitDesc;

// if it returns false, the output parameters are set to 0
// if it returns true, the output parameters are set:
// outSide: pointer to variable to store which side of a wall it hit
//          0 - horizontal side
//          1 - vertical side
// outPointOfCollision: collision of point
extern bool M_RayCollision(map_t* map, vertex2d_t rayOrigin, vertex2d_t rayDir, RayHitDesc* outSide, vertex2d_t* outPointOfCollision);

// safe to pass a map with NULL data and/or NULL filePath
extern void M_Free(map_t* map);

#endif