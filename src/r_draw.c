#include "r_draw.h"

#include <SDL2/SDL.h>
#include <float.h>
#include <stdio.h>
#include <math.h>

#include "v_funcs.h"
#include "p_player.h"
#include "map.h"
#include "e_entity.h"
#include "e_entitymanager.h"
#include "texturebank.h"
#include "r_renderer.h"

#include "logger.h"
/* FORWARD DECLARATIONS */
texture_t* R_UpdateMinimap(renderer_t* render, player_t* player, entitymanager_t* em, map_t* map);
void R_FormVerticesForCircleFromTexture(SDL_Vertex** vertices, int* pNumVertices, unsigned int numTriangles, float angle, vertex2d_t screenPos, float screenRadius, vertex2d_t texturePos, float textureRadius);

/* PUBLIC FUNCTIONS*/
void R_RenderPlayerView(renderer_t* render, player_t* player, map_t* map)
{
    if(!render)
    {
        LogMsg(WARN, "passed null ptr to renderer\n");
        return;
    }

    if(!player)
    {
        LogMsg(WARN, "passed null ptr to player\n");
        return;
    }

    if(!map)
    {
        LogMsg(WARN, "passed null ptr to map\n");
        return;
    }

    texturebank_t*  texturebank = &render->textureBank;
    window_t*       window = render->parentWindow;

    vertex2d_t playerDir;
    vertex2d_t plane;

    playerDir   = V_AngToVec(player->viewAngle);
    plane = V_GetPerpendicular(playerDir);
    V_SetMagnitude(&plane, -tanf(player->fov / 2.f));

    texture_t* textures[10] = { 0 };

    for(int i = 0; i < (int)(sizeof(textures) / sizeof(texture_t*)); i++)
    {
        textures[i] = TB_GetTextureByIndex(texturebank, i);
    }

    for(int x = 0; x < window->width; x++)
    {
        float cameraX = (float)x / (float)window->width * 2.0f - 1.0f;
        vertex2d_t rayDir = V_Add(playerDir, V_Mul(plane, cameraX));

        int mapX = (int)player->pos.x;
        int mapY = (int)player->pos.y;

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
            sideDist.x = (player->pos.x - mapX) * deltaDist.x;
        }
        else
        {
            stepX = 1;
            sideDist.x = (mapX + 1.0f - player->pos.x) * deltaDist.x;
        }

        if(rayDir.y < 0)
        {
            stepY = -1;
            sideDist.y = (player->pos.y - mapY) * deltaDist.y;
        }
        else
        {
            stepY = 1;
            sideDist.y = (mapY + 1.0f - player->pos.y) * deltaDist.y;
        }
        
        while(hit == 0)
        {
            // checking VERTICAL lines
            if(sideDist.x < sideDist.y)
            {
                sideDist.x += deltaDist.x;
                mapX += stepX;
                side = 0;
            }
            // checking HORIZONTAL lines
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

        // TODO: resize depth buffer as window resizes
        if(x < render->depthBufferCount && perpWallDist >= render->depthBuffer[x])
        {
            continue;
        }

        render->depthBuffer[x] = perpWallDist;

        int lineHeight = (int)((float)window->height / perpWallDist);

        int drawStart = -lineHeight / 2 + window->height / 2;
        
        // index into the texture linked list
        int texNum = M_GetMapCell(map, mapY * map->mapHeight + mapX) - 1;

        if(texNum < 0)
        {
            // LogMsg(ERROR, "couldnt find texture\n");
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
            wallX = player->pos.y + perpWallDist * rayDir.y;
        }
        else
        {
            wallX = player->pos.x + perpWallDist * rayDir.x;
        }
        wallX -= floor(wallX);

        int texX = (int)(wallX * (double)wallTexture->width);
        // TB_GetTextureByIndex wont fail since already checked before
        if(side == 0 && rayDir.x > 0) texX = wallTexture->width - texX - 1;
        if(side == 1 && rayDir.y < 0) texX = wallTexture->width - texX - 1;

        SDL_Rect src = {texX, 0, 1, wallTexture->height};
        SDL_Rect dest = {x, drawStart, 1, lineHeight};        

        R_RenderTexture(render, wallTexture, src, dest);
        if(!side)
        {
            SDL_SetRenderDrawBlendMode(window->sdlRenderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(window->sdlRenderer, 0, 0, 0, 0x10);
            SDL_RenderDrawRect(window->sdlRenderer, &dest);
            SDL_SetRenderDrawBlendMode(window->sdlRenderer, SDL_BLENDMODE_NONE);
        }
    }
}

void R_RenderPlayerGun(renderer_t* render, player_t* player)
{
    if(!render)
    {
        LogMsg(WARN, "passed null ptr to renderer\n");
        return;
    }

    if(!player)
    {
        LogMsg(WARN, "passed null ptr to player\n");
        return;
    }

    window_t*       window = render->parentWindow;
    texturebank_t*  texturebank = &render->textureBank;

    gun_t gun = player->currentGun;
    texture_t* weaponTex = NULL;

    switch(gun)
    {
    case FISTS:
        weaponTex = TB_FindTextureByName(texturebank, "FIST");
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

    dstRect.x += sinf(player->gunSway) * weaponTex->width * 0.5f;
    dstRect.y -= fabs(sinf(player->gunSway)) * weaponTex->height * 0.3f * ratio;
    
    if(false)
    {
        SDL_SetRenderDrawColor(window->sdlRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRectF(window->sdlRenderer, &dstRect);
    }

    SDL_RenderCopyF(window->sdlRenderer, weaponTex->data, NULL, &dstRect);
}

void R_RenderCeilingAndFloor(renderer_t* render, SDL_Color topColour, SDL_Color bottomColour)
{
    if(!render)
    {
        LogMsg(WARN, "passed null ptr to renderer\n");
        return;
    }

    window_t* window = render->parentWindow;

    SDL_Rect dest = {0, 0, window->width, window->height / 2};

    #define expandColour(sdlColour) sdlColour.r, sdlColour.g, sdlColour.b, sdlColour.a 

    SDL_SetRenderDrawColor(window->sdlRenderer, expandColour(topColour));
    SDL_RenderFillRect(window->sdlRenderer, &dest);
    dest.y = window->height / 2;
    SDL_SetRenderDrawColor(window->sdlRenderer, expandColour(bottomColour));
    SDL_RenderFillRect(window->sdlRenderer, &dest);

    #undef expandColour
}

void R_RenderMinimap(renderer_t* render, player_t* player, entitymanager_t* em,  map_t* map)
{
    if(!render)
    {
        LogMsg(WARN, "passed null ptr to renderer\n");
        return;
    }

    if(!player)
    {
        LogMsg(WARN, "passed null ptr to player\n");
        return;
    }

    if(!map)
    {
        LogMsg(WARN, "passed null ptr to map\n");
        return;
    }

    if(!em)
    {
        LogMsg(WARN, "passed null ptr to entity manager\n");
        return;
    }

    window_t*       window = render->parentWindow;
    texture_t*      minimapTex = TB_FindTextureByName(&render->textureBank, "MINIMAP");

    if(!minimapTex)
    {
        LogMsg(ERROR, "Failed to find minimap texture. did you forget to add an empty texture called MINIMAP to the games texturebank?\n");
        return;
    }
    
    float minimapRadius = window->width / 12;
    vertex2d_t texturePos = (vertex2d_t){0.5f, 0.5f};
    
    SDL_Vertex* vertices = NULL;
    int numVertices = 0;
    R_FormVerticesForCircleFromTexture(&vertices, &numVertices, 20, 0, (vertex2d_t){minimapRadius + 10, minimapRadius + 10}, minimapRadius, texturePos, 0.5f);

    if(!vertices)
    {
        LogMsg(ERROR, "failed to get vertices for minimap\n");
        return;
    }

    if(SDL_RenderGeometry(window->sdlRenderer, minimapTex->data, vertices, numVertices, NULL, 0) < 0) 
        LogMsgf(ERROR, "SDL_RenderGeometry failed to render minimap. SDL_ERROR:%s\n", SDL_GetError());

    free(vertices);
}

// updates the minimap texture and returns it if successful
// renders the map, player and entities to the minimap texture
texture_t* R_UpdateMinimap(renderer_t* render, player_t* player, entitymanager_t* em, map_t* map)
{
    if(!render)
    {
        LogMsg(WARN, "passed null ptr to renderer\n");
        return NULL;
    }

    if(!player)
    {
        LogMsg(WARN, "passed null ptr to player\n");
        return NULL;
    }

    if(!map)
    {
        LogMsg(WARN, "passed null ptr to map\n");
        return NULL;
    }

    if(!em)
    {
        LogMsg(WARN, "passed null ptr to entity manager\n");
        return NULL;
    }

    texturebank_t*  texturebank = &render->textureBank;
    window_t*       window = render->parentWindow;

    texture_t* minimapTex = TB_FindTextureByName(texturebank, "MINIMAP"), *playerTex = TB_FindTextureByName(texturebank, "player");
    if(!minimapTex)
    {
        LogMsg(ERROR, "Failed to find target minimap texture. did you forget to add a blank texture called 'MINIMAP' to game texturebank?\n");
        return NULL;
    }
    
    if(!playerTex)
    {
        LogMsg(ERROR, "Failed to find target player texture\n");
        return NULL;
    }

    // clear minimap first
    SDL_SetRenderTarget(window->sdlRenderer, minimapTex->data);
    SDL_SetRenderDrawColor(window->sdlRenderer, 0, 0, 0, 255);
    SDL_RenderClear(window->sdlRenderer);

    SDL_Texture* cell = SDL_CreateTexture(window->sdlRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1, 1);

    SDL_SetRenderTarget(window->sdlRenderer, cell);
    SDL_SetRenderDrawColor(window->sdlRenderer, 0, 0, 0xff, 0xff);
    SDL_RenderClear(window->sdlRenderer);

    float       rectWidth;
    float       rectHeight;
    int         x;
    int         y;

    rectWidth   = minimapTex->width / map->mapWidth;
    rectHeight  = minimapTex->height / map->mapHeight;

    /* DRAW WALLS ON MINIMAP */

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

            SDL_SetRenderTarget(window->sdlRenderer, cell);
            bool blue = M_GetMapCell(map, y * map->mapWidth + x) > 0;
            if(blue)
                SDL_SetRenderDrawColor(window->sdlRenderer, 0, 0, 255, 255);
            else
                SDL_SetRenderDrawColor(window->sdlRenderer, 0, 0, 0, 255);            

            SDL_RenderClear(window->sdlRenderer);
            SDL_SetRenderTarget(window->sdlRenderer, minimapTex->data);

            SDL_Point centreRotation = (SDL_Point){minimapTex->width / 2 - cellScreenRect.x, minimapTex->height / 2 - cellScreenRect.y};

            if(SDL_RenderCopyEx(window->sdlRenderer, cell, NULL, &cellScreenRect, -(player->viewAngle + M_PI / 2) * (180 / M_PI), &centreRotation, (SDL_RendererFlip)0) < 0)
                LogMsg(ERROR, "failed to render map cell on minimap texture\n");
        }
    }
    SDL_SetRenderDrawBlendMode(window->sdlRenderer, SDL_BLENDMODE_NONE);
    
    /* DRAW OTHER ENTITIES ON MINIMAP */
    SDL_SetRenderTarget(window->sdlRenderer, cell);
    SDL_SetRenderDrawColor(window->sdlRenderer, 255, 0, 0, 255);
    SDL_RenderClear(window->sdlRenderer);
    SDL_SetRenderTarget(window->sdlRenderer, minimapTex->data);
    for(entity_t* i = em->entities; i - em->entities < em->numEntities; i++)
    {
        SDL_Rect entityScreenRect = {0};

        entityScreenRect.x = i->pos.x * rectWidth + minimapTex->width / 2 - player->pos.x * rectWidth;
        entityScreenRect.y = i->pos.y * rectHeight + minimapTex->height / 2 - player->pos.y * rectHeight;

        entityScreenRect.w = rectWidth;
        entityScreenRect.h = rectHeight;

            SDL_Point centreRotation = (SDL_Point){minimapTex->width / 2 - entityScreenRect.x, minimapTex->height / 2 - entityScreenRect.y};

        SDL_RenderCopyEx(window->sdlRenderer, cell, NULL, &entityScreenRect, -(player->viewAngle + M_PI / 2) * (180 / M_PI), &centreRotation, (SDL_RendererFlip)0);
    }

    /* DRAW PLAYER ON MINIMAP */

    SDL_Rect playerRect = (SDL_Rect){minimapTex->width / 2 - 2, minimapTex->height / 2 - 2, 4, 4};
    SDL_SetRenderDrawColor(window->sdlRenderer, 0, 255, 0, 255);
    SDL_RenderDrawRect(window->sdlRenderer, &playerRect);

    SDL_SetRenderTarget(window->sdlRenderer, NULL);

    SDL_DestroyTexture(cell);

    return minimapTex;
}

bool R_RenderTexture(renderer_t* render, texture_t* tex, SDL_Rect src, SDL_Rect dst)
{
    if(!render)
    {
        LogMsg(ERROR, "passed null ptr to renderer\n");
        return false;
    }

    if(!tex)
    {
        LogMsg(ERROR, "passed null ptr to tex\n");
        return false;
    }

    if(!TB_IsTexInTextureBank(&render->textureBank, tex))
    {
        LogMsg(WARN, "failed to render texture: texture doesn't belong to the destination renderer");
        return false;
    }

    if(SDL_RenderCopy(render->parentWindow->sdlRenderer, tex->data, &src, &dst) < 0)
    {
        LogMsgf(WARN, "failed to render texture: SDL_ERROR:%s\n", SDL_GetError());
        return false;
    }

    return true;
}

/* PRIVATE FUNCTIONS */

// renders the map to a texture called "debugMinimap" in the renderer's texture bank
// if the texture doesn't already exist, it gets created and added to the texture bank
//      with default size of 256x256
// returns true on success
bool R_DebugMinimap(renderer_t* render, map_t* map, player_t* player, entitymanager_t* em)
{
    if(!render)
    {
        LogMsg(ERROR, "passed null ptr to renderer\n");
        return false;
    }

    if(!map)
    {
        LogMsg(ERROR, "passed null ptr to map\n");
        return false;
    }

    texture_t* dbgMinimapTex = TB_FindTextureByName(&render->textureBank, "debugMinimap");
    if(!dbgMinimapTex)
    {
        dbgMinimapTex = TB_AddEmptyTexture(&render->textureBank);
        if(!dbgMinimapTex)
        {
            LogMsg(ERROR, "failed to create debugMinimap texture\n");
            return false;
        }

        *dbgMinimapTex = T_CreateBlankTexture(render->parentWindow, "debugMinimap", 256, 256);
        if(!dbgMinimapTex->data)
        {
            LogMsg(ERROR, "failed to create blank texture for debugMinimap\n");
            return false;
        }
    }

    // draw the walls to the minimap texture
    SDL_SetRenderTarget(render->parentWindow->sdlRenderer, dbgMinimapTex->data);
    SDL_SetRenderDrawColor(render->parentWindow->sdlRenderer, 0, 0, 0, 255);
    SDL_RenderClear(render->parentWindow->sdlRenderer);

    int cellWidth = dbgMinimapTex->width / map->mapWidth;
    int cellHeight = dbgMinimapTex->height / map->mapHeight;

    SDL_SetRenderDrawColor(render->parentWindow->sdlRenderer, 0, 0, 255, 255);
    for(int i = 0; i < map->mapHeight; i++)
    {
        for(int j = 0; j < map->mapWidth; j++)
        {
            SDL_Rect cellRect = { j * cellWidth, i * cellHeight, cellWidth, cellHeight };

            if(M_GetMapCell(map, i * map->mapWidth + j) <= 0)
                continue;
            
            SDL_RenderFillRect(render->parentWindow->sdlRenderer, &cellRect);
        }
    }

    // if player passed, draw player
    if(player)
    {
        SDL_SetRenderDrawColor(render->parentWindow->sdlRenderer, 0, 255, 0, 255);
        vertex2d_t playerScreenPos = {
            player->pos.x * cellWidth,
            player->pos.y * cellHeight
        };
        SDL_RenderDrawRect(render->parentWindow->sdlRenderer, &(SDL_Rect){playerScreenPos.x - 2, playerScreenPos.y - 2, 4, 4});
    }
    
    // if entity manager passed, draw entities
    if(em)
    {
        SDL_SetRenderDrawColor(render->parentWindow->sdlRenderer, 255, 0, 0, 255);

        for(entity_t* e = em->entities; e - em->entities < em->numEntities; e++)
        {
            vertex2d_t entityScreenPos = {
                e->pos.x * cellWidth,
                e->pos.y * cellHeight
            };
            SDL_RenderDrawRect(render->parentWindow->sdlRenderer, &(SDL_Rect){entityScreenPos.x - 2, entityScreenPos.y - 2, 4, 4});
        }
    }

    return true;
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