#include "r_draw.h"

#include <SDL2/SDL.h>
#include <float.h>
#include <stdio.h>
#include <math.h>

#include "v_funcs.h"
#include "p_player.h"
#include "settings.h"
#include "m_game.h"

#include "logger.h"

void R_RenderPlayerView(window_t* window, texturebank_t* texturebank, player_t* p, map_t* map)
{
    // check params
    if(!window)
    {
        LogMsg(WARN, "passed null ptr to window\n");
        return;
    }

    if(!texturebank)
    {
        LogMsg(WARN, "passed null ptr to texturebank\n");
        return;
    }
    
    if(!p)
    {
        LogMsg(WARN, "passed null ptr to player\n");
        return;
    }

    if(!map)
    {
        LogMsg(WARN, "passed null ptr to map\n");
        return;
    }

    vertex2d_t playerDir;
    vertex2d_t plane;
    int x;

    playerDir   = V_AngToVec(p->viewAngle);
    plane       = V_Mul(V_Normalise(V_GetPerpendicular(playerDir)), -1);

    texture_t* textures[10] = { 0 };

    for(int i = 0; i < (int)(sizeof(textures) / sizeof(texture_t*)); i++)
    {
        textures[i] = TB_GetTextureByIndex(texturebank, i);
    }

    for(x = 0; x < window->width; x++)
    {
        float cameraX = (float)x / (float)window->width * 2.0f - 1.0f;
        vertex2d_t rayDir = V_Add(playerDir, V_Mul(plane, cameraX));

        int mapX = (int)p->pos.x;
        int mapY = (int)p->pos.y;

        vertex2d_t sideDist;

        vertex2d_t deltaDist;
        deltaDist.x = (rayDir.x == 0) ? FLT_MAX : fabs(1 / rayDir.x);
        deltaDist.y = (rayDir.y == 0) ? FLT_MAX : fabs(1 / rayDir.y);
        float perpWallDist;

        int stepX;
        int stepY;

        int hit = 0;
        int side;

        if(rayDir.x < 0)
        {
            stepX = -1;
            sideDist.x = (p->pos.x - mapX) * deltaDist.x;
        }
        else
        {
            stepX = 1;
            sideDist.x = (mapX + 1.0f - p->pos.x) * deltaDist.x;
        }

        if(rayDir.y < 0)
        {
            stepY = -1;
            sideDist.y = (p->pos.y - mapY) * deltaDist.y;
        }
        else
        {
            stepY = 1;
            sideDist.y = (mapY + 1.0f - p->pos.y) * deltaDist.y;
        }

        while(hit == 0)
        {
            if(sideDist.x < sideDist.y)
            {
                sideDist.x += deltaDist.x;
                mapX += stepX;
                side = 0;
            }
            else
            {
                sideDist.y += deltaDist.y;
                mapY += stepY;
                side = 1;
            }
            if(mapX < 0 || mapX >= map->mapWidth || mapY < 0 || mapY >= map->mapHeight)
                break;

            if(M_GetMapCell(map, mapY * map->mapHeight + mapX) > 0) hit = 1;
        }

        if(side == 0)   perpWallDist = (sideDist.x - deltaDist.x);
        else            perpWallDist = (sideDist.y - deltaDist.y);
        
        int lineHeight = (int)((float)window->height / perpWallDist);

        int drawStart = -lineHeight / 2 + window->height / 2;
        
        // index into the texture linked list
        int texNum = M_GetMapCell(map, mapY * map->mapHeight + mapX) - 1;

        if(texNum < 0)
        {
            LogMsg(ERROR, "couldnt find texture\n");
            continue;
        }

        texture_t* wallTexture = textures[texNum];

        if(wallTexture == NULL)
        {
            LogMsg(ERROR, "couldnt find texture\n");
            continue;
        }
        double wallX;

        if(side == 0)
        {
            wallX = p->pos.y + perpWallDist * rayDir.y;
        }
        else
        {
            wallX = p->pos.x + perpWallDist * rayDir.x;
        }
        wallX -= floor(wallX);

        int texX = (int)(wallX * (double)wallTexture->width);
        // TB_GetTextureByIndex wont fail since already checked before
        if(side == 0 && rayDir.x > 0) texX = wallTexture->width - texX - 1;
        if(side == 1 && rayDir.y < 0) texX = wallTexture->width - texX - 1;

        SDL_Rect src = {texX, 0, 1, wallTexture->height};
        SDL_Rect dest = {x, drawStart, 1, lineHeight};        

        SDL_RenderCopy(window->sdlRenderer, wallTexture->data, &src, &dest);
        if(!side)
        {
            SDL_SetRenderDrawBlendMode(window->sdlRenderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(window->sdlRenderer, 0, 0, 0, 0x10);
            SDL_RenderDrawRect(window->sdlRenderer, &dest);
            SDL_SetRenderDrawBlendMode(window->sdlRenderer, SDL_BLENDMODE_NONE);
        }
    }
}

void R_RenderPlayerGun(window_t* window, texturebank_t* texturebank, player_t* p)
{
    if(!window)
    {
        LogMsg(WARN, "passed null ptr to window\n");
        return;
    }

    if(!texturebank)
    {
        LogMsg(WARN, "passed null ptr to texturebank\n");
        return;
    }
    if(!p)
    {
        LogMsg(WARN, "passed null ptr to player\n");
        return;
    }

    gun_t gun = p->currentGun;
    texture_t* weaponTex = NULL;

    switch(gun)
    {
    case FISTS:
        weaponTex = TB_FindTexture(texturebank, "FIST");
        break;
    default:
        weaponTex = NULL;
        break;
    }

    if(!weaponTex)
    {
        LogMsg(WARN, "couldn't find current gun texture\n");
        return;
    }

    float ratio = window->width / (2 * weaponTex->width); 

    vertex2d_t newTexDim = {weaponTex->width * ratio, weaponTex->height * ratio};

    SDL_FRect dstRect = {(float)window->width * 0.6f - newTexDim.x * 0.5f, window->height - newTexDim.y * 0.7f, newTexDim.x, newTexDim.y};

    dstRect.x += sinf(p->gunSway) * weaponTex->width * 0.5f;
    dstRect.y -= fabs(sinf(p->gunSway)) * weaponTex->height * 0.3f * ratio;
    
    SDL_SetRenderDrawColor(window->sdlRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRectF(window->sdlRenderer, &dstRect);
    
    SDL_RenderCopyF(window->sdlRenderer, weaponTex->data, NULL, &dstRect);
}

void R_RenderCeilingAndFloor(window_t* window)
{
    SDL_Rect dest = {0, 0, window->width, window->height / 2};

    SDL_SetRenderDrawColor(window->sdlRenderer, 0x40, 0x40, 0x40, 0xff);
    SDL_RenderFillRect(window->sdlRenderer, &dest);
    dest.y = window->height / 2;
    SDL_SetRenderDrawColor(window->sdlRenderer, 0x60, 0x60, 0x60, 0xff);
    SDL_RenderFillRect(window->sdlRenderer, &dest);
}


// updates the minimap texture
void R_UpdateMinimap(window_t* window, texturebank_t* texturebank, player_t* player, map_t* map)
{
    texture_t* minimapTex = TB_FindTexture(texturebank, "MINIMAP"), *playerTex = TB_FindTexture(texturebank, "player");
    if(!minimapTex)
    {
        LogMsg(ERROR, "Failed to find target minimap texture\n");
        return;
    }
    
    if(!playerTex)
    {
        LogMsg(ERROR, "Failed to find target player texture\n");
        return;
    }

    SDL_Texture* blueCell = SDL_CreateTexture(window->sdlRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1, 1);
    SDL_Texture* blackCell = SDL_CreateTexture(window->sdlRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1, 1);
    
    SDL_SetRenderTarget(window->sdlRenderer, blueCell);
    SDL_SetRenderDrawColor(window->sdlRenderer, 0, 0, 0xff, 0xff);
    SDL_RenderClear(window->sdlRenderer);


    SDL_SetRenderTarget(window->sdlRenderer, blackCell);
    SDL_SetRenderDrawColor(window->sdlRenderer, 0, 0, 0, 0xff);
    SDL_RenderClear(window->sdlRenderer);


    SDL_SetRenderTarget(window->sdlRenderer, minimapTex->data);
    SDL_SetRenderDrawColor(window->sdlRenderer, 0, 0, 0, 0xff);
    SDL_RenderClear(window->sdlRenderer);

    float       rectWidth;
    float       rectHeight;
    int         x;
    int         y;

    rectWidth   = minimapTex->width / map->mapWidth;
    rectHeight  = minimapTex->height / map->mapHeight;

    SDL_SetRenderDrawBlendMode(window->sdlRenderer, SDL_BLENDMODE_BLEND);
    for(y = 0; y < map->mapHeight; y++)
    {
        for(x = 0; x < map->mapWidth; x++)
        {
            SDL_Rect cellScreenRect = { 0 };

            cellScreenRect.x = x * rectWidth + minimapTex->width / 2 - player->pos.x * rectWidth;
            cellScreenRect.y = y * rectHeight + minimapTex->height / 2 - player->pos.y * rectHeight;

            cellScreenRect.w = rectWidth;
            cellScreenRect.h = rectHeight;

            bool blue = M_GetMapCell(map, y * map->mapWidth + x) > 0;

            SDL_Point centreRotation = (SDL_Point){minimapTex->width / 2 - cellScreenRect.x, minimapTex->height / 2 - cellScreenRect.y};

            if(SDL_RenderCopyEx(window->sdlRenderer, blue ? blueCell : blackCell, NULL, &cellScreenRect, -(player->viewAngle + M_PI / 2) * (180 / M_PI), &centreRotation, (SDL_RendererFlip)0) < 0)
                LogMsg(ERROR, "failed to render map cell on minimap texture\n");
        }
    }
    SDL_SetRenderDrawBlendMode(window->sdlRenderer, SDL_BLENDMODE_NONE);

    SDL_RenderCopy(window->sdlRenderer, playerTex->data, NULL, &(SDL_Rect){(int)(minimapTex->width / 2) - 2, (int)(minimapTex->height / 2) - 2, 4, 4});

    SDL_SetRenderTarget(window->sdlRenderer, NULL);

    SDL_DestroyTexture(blackCell);
    SDL_DestroyTexture(blueCell);
}

void R_FormVerticesForCircleFromTexture(SDL_Vertex** vertices, int* pNumVertices, unsigned int numTriangles, float angle, vertex2d_t screenPos, float screenRadius, vertex2d_t texturePos, float textureRadius);
void R_RenderMinimap(window_t* window, texturebank_t* texturebank, player_t* p, map_t* map)
{
    p = p; // to stop warning
    R_UpdateMinimap(window, texturebank, p, map);
    texture_t* minimapTex = TB_FindTexture(texturebank, "MINIMAP");
    if(!minimapTex)
    {
        LogMsg(ERROR, "Failed to find minimap texture\n");
        return;
    }
    
    float minimapRadius = window->width / 12;
    vertex2d_t texturePos = (vertex2d_t){0.5f, 0.5f};
    
    SDL_Vertex* vertices = NULL;
    int numVertices = 0;
    R_FormVerticesForCircleFromTexture(&vertices, &numVertices, 20, 0, (vertex2d_t){minimapRadius + 10, minimapRadius + 10}, minimapRadius, texturePos, 0.5f);

    if(!vertices)

    if(SDL_RenderGeometry(window->sdlRenderer, minimapTex->data, vertices, numVertices, NULL, 0) < 0) 
        LogMsgf(ERROR, "SDL_RenderGeometry failed to render minimap. SDL_ERROR:%s\n", SDL_GetError());
}


/// @brief forms vertices to use for SDL_RenderGeometry for rendering a circle from a texture to a circle on screen
/// @param vertices pointer to vertex array. result will be stored here. if it points to already allocated memory, it frees that memory
/// @param numTriangles number of triangles to use for the circle. higher number = more defined circle. must be at least 4
/// @param angle for the texture to be able to rotate the circle on the texture
/// @param screenPos screen position of the centre of the circle to be rendered
/// @param screenRadius radius of the circle on screen
/// @param texturePos centre position of the circle to take from texture. both components has to be in range 0-1 as tex coords are normalised
/// @param textureRadius radius of circle to take from texture. must be in within the range of texture since texture coordinates are normalised 
void R_FormVerticesForCircleFromTexture(SDL_Vertex** vertices, int* pNumVertices, unsigned int numTriangles, float angle, vertex2d_t screenPos, float screenRadius, vertex2d_t texturePos, float textureRadius)
{
    if(numTriangles < 4)
    {
        LogMsg(ERROR, "couldnt form vertices for circle, numTriangles has to be at least 4\n");
        return;
    }

    if(texturePos.x < 0 || texturePos.x > 1 || texturePos.y < 0 || texturePos.y > 1)
    {
        LogMsg(ERROR, "texture coords are outside of the texture coordinates\n");
        return;
    }

    if(!vertices)
    {
        LogMsg(WARN, "passed null ptr to vertices\n");
        return;
    }

    if(!pNumVertices)
    {
        LogMsg(WARN, "passed null ptr to pNumVertices\n");
        return;
    }

    if(*vertices)
        free(*vertices);

    *vertices = malloc(sizeof(SDL_Vertex) * numTriangles * 3);
    if(!*vertices)
    {
        LogMsg(ERROR, "failed to allocate memory for result vertices array\n");
        return;
    }
    
    const int numVertices = numTriangles * 3;

    if(pNumVertices)
        *pNumVertices = numVertices;

    for(int i = 0; i < numVertices; i+= 3)
    {
        int j = i / 3;
        float triangleAngle = (M_PI * 2) / (float)numTriangles;

        SDL_Vertex* centreVertex = &(*vertices)[i];
        SDL_Vertex* currentVertex = &(*vertices)[i + 1];
        SDL_Vertex* finalVertex = &(*vertices)[i + 2];

        centreVertex->position.x = screenPos.x;
        centreVertex->position.y = screenPos.y;

        centreVertex->tex_coord.x = texturePos.x;
        centreVertex->tex_coord.y = texturePos.y;

        centreVertex->color = (SDL_Color){255, 255, 255, 255};

        currentVertex->position.x = cosf(triangleAngle * j) * screenRadius + screenPos.x;
        currentVertex->position.y = sinf(triangleAngle * j) * screenRadius + screenPos.y;

        currentVertex->tex_coord.x = cosf(triangleAngle * j + angle) * textureRadius + texturePos.x;
        currentVertex->tex_coord.y = sinf(triangleAngle * j + angle) * textureRadius + texturePos.y;
        currentVertex->color = (SDL_Color){255, 255, 255, 255};

        finalVertex->position.x = cosf(triangleAngle * (j + 1)) * screenRadius + screenPos.x;
        finalVertex->position.y = sinf(triangleAngle * (j + 1)) * screenRadius + screenPos.y;
        
        finalVertex->tex_coord.x = cosf(triangleAngle * (j + 1) + angle) * textureRadius + texturePos.x;
        finalVertex->tex_coord.y = sinf(triangleAngle * (j + 1) + angle) * textureRadius + texturePos.y;
        
        finalVertex->color = (SDL_Color){255, 255, 255, 255};
    }
}