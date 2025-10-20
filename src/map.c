#include "map.h"
#include "stdio.h"
#include <stdlib.h>
#include "string.h"
#include "logger.h"

#include <stdbool.h>

bool M_FillMapData(map_t* map, FILE* file);

void M_LoadMap(map_t* map, const char* filePath)
{
    if(!map)
    {
        LogMsg(WARN, "passed null pointer to M_LoadMap\n");
        return;
    }

    FILE*       file;
    char        buffer[1024];

    file = fopen(filePath, "r");

    if(!file)
    {
        LogMsgf(ERROR, "Failed to open map file: %s\n", filePath);
        return;
    }

    map->filePath = malloc(strlen(filePath) + 1);
    if(!map->filePath)
    {
        LogMsg(ERROR, "Failed to allocate memory for map file path\n");
        fclose(file);
        return;
    }
    strcpy(map->filePath, filePath);
    map->filePath[strlen(filePath)] = '\0';

    while(!feof(file))
    {
        memset(buffer, 0, 1024);
        fgets(buffer, 1024, file);

        if(buffer[0] == '#' || buffer[0] == '\n' || buffer[0] == '\r')
            continue;

        char* token = strtok(buffer, " \t\n\r");
        if(!token)
            continue;
        
        // parse map data

        // set map dimensions
        if(strcmp(token, "mapdim") == 0)
        {
            // check map dimensions are correct
            map->mapWidth = atoi(strtok(NULL, " \t\n\r"));
            map->mapHeight = atoi(strtok(NULL, " \t\n\r"));
            if(map->mapWidth <= 0 || map->mapHeight <= 0)
            {
                LogMsgf(ERROR, "Invalid map dimensions in map file %s\n", filePath);
                fclose(file);
                return;
            }

            // allocate memory for map data
            map->mapData = malloc(sizeof(int) * map->mapWidth * map->mapHeight);
            if(!map->mapData)
            {
                LogMsgf(ERROR, "Failed to allocate memory for map data (%d x %d) in map file %s\n", map->mapWidth, map->mapHeight, filePath);
                fclose(file);
                return;
            }

            // debug info
            LogMsgf(DEBUG, "Map dimensions: %d x %d\n", map->mapWidth, map->mapHeight);
            memset(map->mapData, 0, sizeof(int) * map->mapWidth * map->mapHeight);
            continue;
        }

        // read map data
        if(strcmp(token, "mapstart") == 0)
        {
            if(!M_FillMapData(map, file))
            {
                LogMsgf(ERROR, "Failed to read map data in map file %s\n", filePath);
                fclose(file);
                M_Free(map);
                return;
            }

            continue;
        }

        if(strcmp(token, "fileend") == 0)
        {
            LogMsgf(DEBUG, "Reached end of map data in map file %s\n", filePath);
            break;
        }
    }

    fclose(file);

    return;
}

// doesn't handle freeing of map file path, map data or of file closing
bool M_FillMapData(map_t* map, FILE* file)
{
    LogMsgf(DEBUG, "Filling map data for map file %s\n", map->filePath);

    char* buffer = malloc(map->mapWidth + 2); // +2 for newline and null terminator
    if (!buffer)
    {
        LogMsg(ERROR, "Failed to allocate memory for reading map data buffer\n");
        return false;
    }
    
    if(map->mapData == NULL)
    {
        LogMsgf(ERROR, "Map dimensions must be specified before map data in map file %s\n", map->filePath);
        free(buffer);
        return false;
    }

    int y = 0;
    while(y < map->mapHeight && !feof(file))
    {
        fgets(buffer, map->mapWidth + 2, file);

        if(buffer[0] == '#' || buffer[0] == '\n' || buffer[0] == '\r')
            continue;

        if((int)strlen(buffer) < map->mapWidth)
        {
            LogMsgf(ERROR, "Map data row %d is too short in map file %s\n", y, map->filePath);
            free(buffer);
            return false;
        }

        for(int x = 0; x < map->mapWidth; x++)
        {
            if(buffer[x] < '0' || buffer[x] > '9')
            {
                LogMsgf(ERROR, "Invalid character '%c' in map data at row %d column %d in map file %s\n", buffer[x], y, x, map->filePath);
                free(buffer);
                return false;
            }
            map->mapData[y * map->mapWidth + x] = buffer[x] - '0';
        }

        y++;
    }
    
    if(y < map->mapHeight)
    {
        LogMsgf(ERROR, "Not enough rows of map data in map file %s\n", map->filePath);
        free(buffer);
        return false;
    }

    free(buffer);
    return true;
}

void M_Free(map_t* map)
{
    if(map->mapData)
        free(map->mapData);
    if(map->filePath)
        free(map->filePath);
}

int M_GetMapCell(map_t* map, int index)
{
    if(!map || !map->mapData)
    {
        LogMsg(WARN, "Passing null or uninitialised map to M_GetMapCell()\n");
        return -1;
    }

    if(index < 0 || index > map->mapHeight * map->mapWidth - 1)
        return -1;

    return map->mapData[index];
}
