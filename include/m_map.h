#ifndef __MAP_H__
#define __MAP_H__

typedef struct map_s
{
    int*    mapData;
    int     mapWidth;
    int     mapHeight;

    char*   filePath;
} map_t;

extern map_t M_LoadMap(const char* filePath);
// safe to pass a map with NULL data and/or NULL filePath
extern  void M_Free(map_t map);

#endif