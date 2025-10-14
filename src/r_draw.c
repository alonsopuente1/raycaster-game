#include "r_draw.h"

#include <SDL2/SDL.h>
#include <float.h>
#include <stdio.h>
#include <math.h>

#include "i_init.h"
#include "v_funcs.h"
#include "p_player.h"
#include "settings.h"
#include "t_textures.h"
#include "w_window.h"

#include "logger.h"

extern window_t gMainWindow;

extern SDL_Texture* playerTex;

void R_RenderPlayerView(player_t* p, map_t* map)
{
    vertex2d_t playerDir;
    vertex2d_t plane;
    int x;

    playerDir   = V_AngToVec(p->viewAngle);
    plane       = V_Mul(V_Normalise(V_GetPerpendicular(playerDir)), -1);

    for(x = 0; x < gMainWindow.width; x++)
    {
        float cameraX = (float)x / (float)gMainWindow.width * 2.0f - 1.0f;
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
            if(map->mapData[mapY * map->mapHeight + mapX] > 0) hit = 1;
        }

        if(side == 0)   perpWallDist = (sideDist.x - deltaDist.x);
        else            perpWallDist = (sideDist.y - deltaDist.y);
        

        int lineHeight = (int)((float)gMainWindow.height / perpWallDist);

        int drawStart = -lineHeight / 2 + gMainWindow.height / 2;

        SDL_Colour colour = {0};
        switch(map->mapData[mapY * map->mapHeight + mapX])
        {
        case 1:
            colour.r = 0xff;
            colour.g = 0;
            colour.b = 0;
            break;
        case 2:
            colour.r = 0; 
            colour.g = 0xff, 
            colour.b = 0;
            break;
        case 3:
            colour.r = 0;
            colour.g = 0; 
            colour.b = 0xff;
            break;
        case 4:
            colour.r = 0xff; 
            colour.g = 0xff;
            colour.b = 0;
            break;
        case 5:
            colour.r = 0;
            colour.g = 0xff;
            colour.b = 0xff;
            break;
        default:
            colour.r = 0;
            colour.g = 0xff;
            colour.b = 0xff;
            break;
        }

        if(side == 1)
        {
            colour.r /= 2;
            colour.g /= 2;
            colour.b /= 2;
        }
        
        // index into the texture array 
        int texNum = map->mapData[mapY * map->mapHeight + mapX] - 1;

        if(texNum < 0 || texNum >= NUMTEXTURES)
            texNum = 0;

        // if(texNum > 0)
        // {
        //     SDL_SetRenderDrawColor(gMainWindow.sdlRenderer, colour.r, colour.g, colour.b, 0xff);
        //     SDL_RenderDrawLine(gMainWindow.sdlRenderer, x, drawStart, x, drawEnd);
        // }
        // else
        // {
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

            int texX = (int)(wallX * (double)gTextures[texNum].width);
            if(side == 0 && rayDir.x > 0) texX = gTextures[texNum].width - texX - 1;
            if(side == 1 && rayDir.y < 0) texX = gTextures[texNum].width - texX - 1;

            SDL_Rect src = {texX, 0, 1, gTextures[texNum].height};
            SDL_Rect dest = {x, drawStart, 1, lineHeight};        

            SDL_RenderCopy(gMainWindow.sdlRenderer, gTextures[texNum].data, &src, &dest);
        // }
    }
}

void R_RenderPlayerGun(player_t* p)
{
    gun_t gun = p->currentGun;
    texture_t* weaponTex = NULL;

    if(gun == FISTS)
    {
        weaponTex = T_FindTexture("FIST");
    }

    if(weaponTex == NULL)
    {
        LogMsg(WARN, "Can't find weapon texture\n");
    }

    float ratio = gMainWindow.width / (2 * weaponTex->width); 

    vertex2d_t newTexDim = {weaponTex->width * ratio, weaponTex->height * ratio};

    SDL_FRect dstRect = {(float)gMainWindow.width * 0.6f - newTexDim.x * 0.5f, gMainWindow.height - newTexDim.y * 0.7f, newTexDim.x, newTexDim.y};

    dstRect.x += sinf(p->gunSway) * weaponTex->width * 0.5f;
    dstRect.y -= fabs(sinf(p->gunSway)) * weaponTex->height * 0.3f * ratio;
    
    SDL_SetRenderDrawColor(gMainWindow.sdlRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRectF(gMainWindow.sdlRenderer, &dstRect);
    
    SDL_RenderCopyF(gMainWindow.sdlRenderer, weaponTex->data, NULL, &dstRect);
}

void R_RenderMap(player_t* p, map_t* map)
{
    float       rectWidth;
    float       rectHeight;
    int         x;
    int         y;

    rectWidth   = gMainWindow.width / map->mapWidth;
    rectHeight  = gMainWindow.height / map->mapHeight;

    SDL_SetRenderDrawBlendMode(gMainWindow.sdlRenderer, SDL_BLENDMODE_BLEND);
    for(y = 0; y < map->mapHeight; y++)
    {
        for(x = 0; x < map->mapWidth; x++)
        {
            SDL_Rect rect = {x * rectWidth, y * rectHeight, rectWidth, rectHeight};
            if(map->mapData[y * map->mapWidth + x])
                SDL_SetRenderDrawColor(gMainWindow.sdlRenderer, 0, 0, 0xff, 0x33);
            else
                SDL_SetRenderDrawColor(gMainWindow.sdlRenderer, 0, 0, 0, 0x33);
            SDL_RenderFillRect(gMainWindow.sdlRenderer, &rect);
            SDL_SetRenderDrawColor(gMainWindow.sdlRenderer, 0xff, 0, 0, 0x33);
            SDL_RenderDrawRect(gMainWindow.sdlRenderer, &rect);
        }
    }
    SDL_SetRenderDrawBlendMode(gMainWindow.sdlRenderer, SDL_BLENDMODE_NONE);

    vertex2d_t screenPos = {(gMainWindow.width * (p->pos.x / (float)map->mapWidth) ) - 5.f, (gMainWindow.height * (p->pos.y / (float)map->mapHeight)) - 5.f};
    SDL_Rect dest = {screenPos.x, screenPos.y, 10, 10};
    SDL_RenderCopy(gMainWindow.sdlRenderer, playerTex, NULL, &dest);

    SDL_SetRenderDrawColor(gMainWindow.sdlRenderer, 0, 0xff, 0, 0x33);
    
    vertex2d_t viewLine = V_Add(V_Mul(V_AngToVec(p->viewAngle), 10), screenPos);

    SDL_RenderDrawLine(gMainWindow.sdlRenderer, (int)screenPos.x, (int)screenPos.y, (int)viewLine.x, (int)viewLine.y);

    SDL_SetRenderDrawColor(gMainWindow.sdlRenderer, 0xff, 0, 0, 0x33);
    vertex2d_t viewLine2 = V_Add(viewLine, V_Mul(V_GetPerpendicular(V_AngToVec(p->viewAngle)), 10));
    SDL_RenderDrawLine(gMainWindow.sdlRenderer, (int)viewLine.x, (int)viewLine.y, (int)viewLine2.x, (int)viewLine2.y);
}

void R_RenderCeilingAndFloor(void)
{
    SDL_Rect dest = {0, 0, gMainWindow.width, gMainWindow.height / 2};

    SDL_SetRenderDrawColor(gMainWindow.sdlRenderer, 0x40, 0x40, 0x40, 0xff);
    SDL_RenderFillRect(gMainWindow.sdlRenderer, &dest);
    dest.y = gMainWindow.height / 2;
    SDL_SetRenderDrawColor(gMainWindow.sdlRenderer, 0x60, 0x60, 0x60, 0xff);
    SDL_RenderFillRect(gMainWindow.sdlRenderer, &dest);
}

void R_RenderCircleFromTexture(texture_t* tex, vertex2d_t texPos, float texRadius, vertex2d_t screenPos, float screenRadius)
{
}