#include "r_draw.h"

#include "i_init.h"
#include "v_funcs.h"
#include "p_player.h"
#include "settings.h"
#include "logger.h"
#include "t_textures.h"

#include <SDL2/SDL.h>
#include <float.h>
#include <stdio.h>

extern SDL_Renderer* gRenderer;

extern int gScreenWidth;
extern int gScreenHeight;

extern SDL_Texture* playerTex;

void R_RenderPlayerView(player_t* p, map_t* map)
{
    vertex2d_t playerDir;
    vertex2d_t plane;
    int x;

    playerDir   = V_AngToVec(p->viewAngle);
    plane       = V_Mul(V_Normalise(V_GetPerpendicular(playerDir)), -1);

    for(x = 0; x < gScreenWidth; x++)
    {
        float cameraX = (float)x / (float)gScreenWidth * 2.0f - 1.0f;
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

        int lineHeight = (int)((float)gScreenHeight / perpWallDist);

        int drawStart = -lineHeight / 2 + gScreenHeight / 2;

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
        //     SDL_SetRenderDrawColor(gRenderer, colour.r, colour.g, colour.b, 0xff);
        //     SDL_RenderDrawLine(gRenderer, x, drawStart, x, drawEnd);
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

            SDL_RenderCopy(gRenderer, gTextures[texNum].data, &src, &dest);
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

    SDL_Rect dstRect = {gScreenWidth - weaponTex->width, gScreenHeight - weaponTex->height, weaponTex->width, weaponTex->height};

    SDL_RenderCopy(gRenderer, weaponTex->data, NULL, &dstRect);
}

void R_RenderMap(player_t* p, map_t* map)
{
    float       rectWidth;
    float       rectHeight;
    int         x;
    int         y;

    rectWidth   = gScreenWidth / map->mapHeight;
    rectHeight  = gScreenHeight / map->mapWidth;

    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
    for(y = 0; y < map->mapHeight; y++)
    {
        for(x = 0; x < map->mapWidth; x++)
        {
            SDL_Rect rect = {x * rectWidth, y * rectHeight, rectWidth, rectHeight};
            if(map->mapData[y * map->mapHeight + x])
                SDL_SetRenderDrawColor(gRenderer, 0, 0, 0xff, 0x33);
            else
                SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0x33);
            SDL_RenderFillRect(gRenderer, &rect);
        }
    }
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);

    vertex2d_t screenPos = {(gScreenWidth * (p->pos.x / (float)map->mapWidth) ) - 5.f, (gScreenHeight * (p->pos.y / (float)map->mapHeight)) - 5.f};
    SDL_Rect dest = {screenPos.x, screenPos.y, 10, 10};
    SDL_RenderCopy(gRenderer, playerTex, NULL, &dest);

    SDL_SetRenderDrawColor(gRenderer, 0, 0xff, 0, 0x33);
    
    vertex2d_t viewLine = V_Add(V_Mul(V_AngToVec(p->viewAngle), 10), screenPos);

    SDL_RenderDrawLine(gRenderer, (int)screenPos.x, (int)screenPos.y, (int)viewLine.x, (int)viewLine.y);

    SDL_SetRenderDrawColor(gRenderer, 0xff, 0, 0, 0x33);
    vertex2d_t viewLine2 = V_Add(viewLine, V_Mul(V_GetPerpendicular(V_AngToVec(p->viewAngle)), 10));
    SDL_RenderDrawLine(gRenderer, (int)viewLine.x, (int)viewLine.y, (int)viewLine2.x, (int)viewLine2.y);
}

void R_RenderCeilingAndFloor(void)
{
    SDL_Rect dest = {0, 0, gScreenWidth, gScreenHeight / 2};

    SDL_SetRenderDrawColor(gRenderer, 0x40, 0x40, 0x40, 0xff);
    SDL_RenderFillRect(gRenderer, &dest);
    dest.y = gScreenHeight / 2;
    SDL_SetRenderDrawColor(gRenderer, 0x60, 0x60, 0x60, 0xff);
    SDL_RenderFillRect(gRenderer, &dest);
}
