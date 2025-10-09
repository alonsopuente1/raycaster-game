#include "m_map.h"
#include "stdio.h"
#include <stdlib.h>
#include "string.h"

map_t M_LoadMap(const char* filePath)
{
    FILE*       file;
    map_t       out = {0};
    char        buffer[1024];

    file = fopen(filePath, "r");

    if(!file)
    {
        printf("Failed to open map file: %s\n", filePath);
        return out;
    }

    fgets(buffer, 1024, file);
    sscanf(buffer, "%i %i", &out.mapWidth, &out.mapHeight);

    out.mapData = (int*)malloc(sizeof(int) * out.mapHeight * out.mapWidth);

    if(!out.mapData)
    {
        printf("Failed to allocate memory for map data\n");
        return out;
    }

    int h = 0;
    while(!feof(file))
    {
        fgets(buffer, 1024, file);
        for(int i = 0; i < out.mapWidth; i++)
        {
            out.mapData[h * out.mapWidth + i] = buffer[i] - '0';
        }
        h++;
    }

    fclose(file);

    return out;
}

void M_Free(map_t map)
{
    free(map.mapData);
}