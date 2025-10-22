#ifndef __MAP_H__
#define __MAP_H__

#include "v_vert.h"

#include <stdbool.h>

typedef struct maploadargs_s
{
    // stores start position of player in map
    vertex2d_t  startPos;

    // true when map loaded successfully
    bool        success;

} maploadargs_t;

typedef struct map_s
{
    // flat 2d array
    int*    mapData;

    int     mapWidth;
    int     mapHeight;

    char*   filePath;
} map_t;

// returns -1 on out of range value, passing NULL to map or passing index out of range of map
extern int M_GetMapCell(map_t* map, int index);
// if mapArgs->success == false then map failed to load
extern void M_LoadMap(map_t* map, maploadargs_t* mapArgs, const char* filePath);
// safe to pass a map with NULL data and/or NULL filePath
extern void M_Free(map_t* map);

#endif