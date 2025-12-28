#include "map.h"
#include "stdio.h"
#include <stdlib.h>
#include "string.h"
#include "logger.h"
#include "v_funcs.h"

#include <stdbool.h>
#include <float.h>
#include <fcntl.h>

bool M_FillMapData(map_t* map, FILE* file);

void M_LoadMap(map_t* map, maploadargs_t* mapArgs, const char* filePath)
{
    #define CLEANUP()   fclose(file);\
                        M_Free(map);\
                        memset(mapArgs, sizeof(maploadargs_t), 0)
                        
    if(!map)
    {
        LogMsg(WARN, "passed null pointer to M_LoadMap\n");
        return;
    }

    if(!mapArgs)
    {
        LogMsg(WARN, "passed null ptr to mapArgs\n");
        return;
    }
    

    FILE*       file = NULL;
    char        buffer[1024] = { 0 };
    
    file = fopen(filePath, "r");
    
    mapArgs->success = false;
    
    if(!file)
    {
        SetErrorf("Failed to open map file: %s", filePath);
        return;
    }

    M_Free(map);
    
    map->filePath = malloc(strlen(filePath) + 1);
    if(!map->filePath)
    {
        LogMsg(ERROR, "no memory");
        exit(-1);
    }
    strncpy(map->filePath, filePath, strlen(filePath));
    map->filePath[strlen(filePath)] = '\0';
    
    int lineNum = 0;
    while(!feof(file))
    {
        lineNum++;
        memset(buffer, 0, sizeof(buffer));
        fgets(buffer, sizeof(buffer), file);
        
        if(buffer[0] == '#' || buffer[0] == '\n' || buffer[0] == '\r' || strlen(buffer) == 0)
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
                SetErrorf("Invalid map dimensions in map file '%s'", filePath);
                CLEANUP();
                return;
            }

            // allocate memory for map data
            map->mapData = malloc(sizeof(int) * map->mapWidth * map->mapHeight);
            if(!map->mapData)
            {
                ShowMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", "no memory");
                exit(-1);
            }

            // debug info
            LogMsgf(DEBUG, "Map dimensions: %d x %d\n", map->mapWidth, map->mapHeight);
            memset(map->mapData, 0, sizeof(int) * map->mapWidth * map->mapHeight);
            continue;
        }

        // read map data
        if(strcmp(token, "mapstart") == 0)
        {
            // the function sets an error on fail so no need to set the error here
            if(!M_FillMapData(map, file))
            {
                CLEANUP();
                return;
            }
            // adjusts for lines skipped by M_FillMapData
            lineNum += map->mapHeight;
            continue;
        }

        /* PLAYER DATA */

        // player start var
        if(strcmp(token, "playerstart") == 0)
        {
            float x = atof(strtok(NULL, " \t\n\r")) , y = atof(strtok(NULL, " \t\n\r"));
            if(x == 0 || y == 0)
            {
                SetError("failed to load playerstart co-ords\n");
                CLEANUP();
                return;
            }

            mapArgs->startPos = V_Make(x, y);
            continue;
        }

        // player max speed var
        if(strcmp(token, "playermaxspeed") == 0)
        {
            float maxSpeed = atof(strtok(NULL, " \t\n\r"));
            if(maxSpeed == 0.0f)
            {
                SetError("failed to load maxSpeed for player");
                CLEANUP();
                return;
            }

            mapArgs->maxSpeed = maxSpeed;
            continue;
        }

        // player rotate speed var
        if(strcmp(token, "playerrotatespeed") == 0)
        {
            float rotateSpeed = atof(strtok(NULL, " \t\n\r"));
            if(rotateSpeed == 0.0f)
                LogMsg(INFO, "playerrotatespeed not set, defaulting to 0.1...\n");

            mapArgs->rotateSpeed = rotateSpeed;
            continue;
        }

        /* FILE END TAG*/
        if(strcmp(token, "fileend") == 0)
        {
            LogMsgf(DEBUG, "Reached end of map data in map file %s\n", filePath);
            break;
        }

        // if code execution reached here, the token is unrecognised
        SetErrorf("Unkown token ('%s') on line %i of map file '%s'", token, lineNum, filePath);
        CLEANUP();
        return;
    }

    fclose(file);

    // check for signs of an actually created map
    // if not created, set error, mark failure and return;
    if(!map->filePath || !map->mapData || map->mapHeight == 0 || map->mapWidth == 0)
    {
        SetError("Unknown error while creating map!\nDoes the map file actually have data in it?");
        CLEANUP();
        mapArgs->success = false;
        return;
    }

    mapArgs->success = true;

    if(mapArgs->rotateSpeed == 0.0f)
        mapArgs->rotateSpeed = 0.0035f;

    #undef CLEANUP

    return;
}

// sets error on fail
// doesn't handle freeing of map file path, map data or of file closing
bool M_FillMapData(map_t* map, FILE* file)
{
    LogMsgf(DEBUG, "Filling map data for map file %s\n", map->filePath);

    char* buffer = malloc(map->mapWidth + 2); // +2 for newline and null terminator
    if (!buffer)
    {
        LogMsg(ERROR, "Failed to allocate memory for reading map data buffer\n");
        exit(-1);
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

        char* sanitised = strtok(buffer, " \n\r\t");

        if((int)strlen(sanitised) != map->mapWidth)
        {
            SetErrorf("Map width doesnt match dimensions set in map file %s\n", map->filePath);
            free(buffer);
            return false;
        }

        for(int x = 0; x < map->mapWidth; x++)
        {
            if(sanitised[x] < '0' || sanitised[x] > '9')
            {
                SetErrorf("Invalid character '%c' in map data at row %d column %d in map file %s\n", buffer[x], y, x, map->filePath);
                free(buffer);
                return false;
            }
            map->mapData[y * map->mapWidth + x] = sanitised[x] - '0';
        }

        y++;
    }
    
    if(y < map->mapHeight)
    {
        SetErrorf("Not enough rows of map data in map file %s\n", map->filePath);
        free(buffer);
        return false;
    }

    free(buffer);
    return true;
}

bool M_RayCollision(map_t* map, vertex2d_t rayOrigin, vertex2d_t rayDir, RayHitDesc* outSide, vertex2d_t* outPointOfCollision)
{
    if(!map)
    {
        LogMsg(ERROR, "passed null ptr to map\n");
        return false;
    }

    if(outSide)
        *outSide = RAY_HIT_NONE;

    int mapX = (int)rayOrigin.x;
    int mapY = (int)rayOrigin.y;

    vertex2d_t sideDist = { 0.f };

    vertex2d_t deltaDist = { 0.f };
    deltaDist.x = (rayDir.x == 0.f) ? FLT_MAX : fabsf(1.f / rayDir.x);
    deltaDist.y = (rayDir.y == 0.f) ? FLT_MAX : fabsf(1.f / rayDir.y);
    float wallDist = 0.f;

    int stepX = 0;
    int stepY = 0;

    int hit = 0;
    RayHitDesc side = RAY_HIT_NONE;

    if(rayDir.x < 0)
    {
        stepX = -1;
        sideDist.x = (rayOrigin.x - mapX) * deltaDist.x;
    }
    else
    {
        stepX = 1;
        sideDist.x = (mapX + 1.0f - rayOrigin.x) * deltaDist.x;
    }

    if(rayDir.y < 0)
    {
        stepY = -1;
        sideDist.y = (rayOrigin.y - mapY) * deltaDist.y;
    }
    else
    {
        stepY = 1;
        sideDist.y = (mapY + 1.0f - rayOrigin.y) * deltaDist.y;
    }
        
    while(hit == 0)
    {
        // checking VERTICAL lines
        if(sideDist.x < sideDist.y)
        {
            sideDist.x += deltaDist.x;
            mapX += stepX;
            side = RAY_HIT_VERTICAL;
        }
        // checking HORIZONTAL lines
        else
        {
            sideDist.y += deltaDist.y;
            mapY += stepY;
            side = RAY_HIT_HORIZONTAL;
        }
        if(mapX < 0 || mapX >= map->mapWidth || mapY < 0 || mapY >= map->mapHeight)
            break;
        int mapVal = M_GetMapCell(map, mapY * map->mapHeight + mapX);
        if(mapVal > 0) hit = 1;
        else if(mapVal < 0) {
            side = RAY_HIT_NONE;
            hit = 0;
            break;
        }
    }

    wallDist = 0.0f; 
    if(side == RAY_HIT_HORIZONTAL)      wallDist = sideDist.y - deltaDist.y;
    else if(side == RAY_HIT_VERTICAL)   wallDist = sideDist.x - deltaDist.x;
    
    // if a wall was hit
    if(hit == 1)
    {
        // set the output parameters
        if(outSide)
            *outSide = side;

        if(outPointOfCollision)
        {
            vertex2d_t normRayDir = V_Normalise(rayDir);

            outPointOfCollision->x = rayOrigin.x + normRayDir.x * wallDist;
            outPointOfCollision->y = rayOrigin.y + normRayDir.y * wallDist;
        }

        return true;
    }
    // no wall hit
    else
    {
        if(outSide)
            *outSide = RAY_HIT_NONE;

        if(outPointOfCollision)
        {
            outPointOfCollision->x = 0.0f;
            outPointOfCollision->y = 0.0f;
        }

        return false;
    }
}

void M_Free(map_t* map)
{
    if(map->mapData)
    {
       free(map->mapData);
       map->mapData = NULL;
    }
    if(map->filePath)
    {
        free(map->filePath);
        map->filePath = NULL;
    }
}

int M_GetMapCell(map_t* map, int index)
{
    if(!map || !map->mapData)
    {
        LogMsg(WARN, "Passed null or uninitialised map\n");
        return -1;
    }

    if(index < 0 || index > map->mapHeight * map->mapWidth - 1)
        return -1;

    return map->mapData[index];
}
