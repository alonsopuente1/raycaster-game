#ifndef __MAP_H__
#define __MAP_H__

typedef struct map_s
{
    int*    mapData;
    int     mapWidth;
    int     mapHeight;
} map_t;

extern map_t M_LoadMap(const char* filePath);
extern  void M_Free(map_t map);

#endif