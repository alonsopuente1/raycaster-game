#include "m_map.h"
#include "stdio.h"
#include <stdlib.h>
#include "string.h"
#include "logger.h"

#include <stdbool.h>

bool M_FillMapData(map_t* map, FILE* file);

map_t M_LoadMap(const char* filePath)
{
    FILE*       file;
    map_t       out = {0};
    char        buffer[1024];

    file = fopen(filePath, "r");

    if(!file)
    {
        LogMsgf(ERROR, "Failed to open map file: %s\n", filePath);
        return out;
    }

    out.filePath = malloc(strlen(filePath) + 1);
    if(!out.filePath)
    {
        LogMsg(ERROR, "Failed to allocate memory for map file path\n");
        fclose(file);
        return out;
    }
    strcpy(out.filePath, filePath);
    out.filePath[strlen(filePath)] = '\0';

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
            out.mapWidth = atoi(strtok(NULL, " \t\n\r"));
            out.mapHeight = atoi(strtok(NULL, " \t\n\r"));
            if(out.mapWidth <= 0 || out.mapHeight <= 0)
            {
                LogMsgf(ERROR, "Invalid map dimensions in map file %s\n", filePath);
                fclose(file);
                out = (map_t){0};
                return out;
            }

            // allocate memory for map data
            out.mapData = malloc(sizeof(int) * out.mapWidth * out.mapHeight);
            if(!out.mapData)
            {
                LogMsgf(ERROR, "Failed to allocate memory for map data (%d x %d) in map file %s\n", out.mapWidth, out.mapHeight, filePath);
                fclose(file);
                out = (map_t){0};
                return out;
            }

            // debug info
            LogMsgf(DEBUG, "Map dimensions: %d x %d\n", out.mapWidth, out.mapHeight);
            memset(out.mapData, 0, sizeof(int) * out.mapWidth * out.mapHeight);
            continue;
        }

        // read map data
        if(strcmp(token, "mapstart") == 0)
        {
            if(!M_FillMapData(&out, file))
            {
                LogMsgf(ERROR, "Failed to read map data in map file %s\n", filePath);
                fclose(file);
                M_Free(out);
                out = (map_t){0};
                return out;
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

    return out;
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

void M_Free(map_t map)
{
    if(map.mapData)
        free(map.mapData);
    if(map.filePath)
        free(map.filePath);
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
