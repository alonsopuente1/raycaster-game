#include "renderer.hpp"

#include "window.hpp"
#include "player.hpp"
#include "map.hpp"

#include "logger.hpp"

#include <cfloat>

void CastEngine::Renderer::ResetDepthBuffer()
{
    depthBuffer.resize(mWindow.GetWidth());
    for(float& f : depthBuffer)
    {
        f = FLT_MAX;
    }
}

CastEngine::Renderer::Renderer(Window &window) : mWindow(window)
{
    ResetDepthBuffer();
}

CastEngine::Renderer::~Renderer()
{
    texBank.FreeAll();
}

bool CastEngine::Renderer::RenderTexture(const Texture &tex, SDL_Rect src, SDL_Rect dst)
{
    if(SDL_RenderCopy(mWindow.GetRenderer(), tex.GetTexture(), &src, &dst) < 0)
    {
        LogMsgf(ERROR, "failed to render texture. SDL_ERROR: %s", SDL_GetError());
        return false;
    }    
    return true;
}

void CastEngine::Renderer::RenderPlayerView(const Player &pPlayer, const Map &pMap)
{
    vec2d playerDir = vec2d::AngToVec(pPlayer.GetViewAng());

    vec2d plane = playerDir.GetPerpendicular();

    const Texture* textures[10] = { 0 };

    vec2d ppos = pPlayer.GetPos();

    for(int i = 0; i < sizeof(textures) / sizeof(textures[0]); i++)
    {
        textures[i] = &texBank[i];
    }

    for(int x = 0; x < mWindow.GetWidth(); x++)
    {
        float cameraX = (float)x / (float)mWindow.GetWidth() * 2.0f - 1.0f;
        vec2d rayDir = playerDir + (plane * cameraX);
        int mapX = (int)ppos.x;
        int mapY = (int)ppos.y;

        vec2d sideDist;

        vec2d deltaDist;
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
            sideDist.x = (ppos.x - mapX) * deltaDist.x;
        }
        else
        {
            stepX = 1;
            sideDist.x = (mapX = 1.0f - ppos.x) * deltaDist.x;
        }

        if(rayDir.y < 0)
        {
            stepY = -1;
            sideDist.y = (ppos.y - mapY) * deltaDist.y;
        }
        else
        {
            stepY = 1;
            sideDist.y = (mapY + 1.0f - ppos.y) * deltaDist.y;
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
            if(mapX < 0 || mapX >= pMap.GetWidth() || mapY < 0 || mapY >= pMap.GetHeight())
                break;

            if(pMap[mapY * pMap.GetHeight() + mapX] > 0) hit = 1;
        }

        if(side == 0)   perpWallDist = (sideDist.x - deltaDist.x);
        else            perpWallDist = (sideDist.y - deltaDist.y);

        if(x < depthBuffer.size() && perpWallDist >= depthBuffer[x])
            continue;

        depthBuffer[x] = perpWallDist;

        int lineHeight = (int)((float)mWindow.GetHeight() / perpWallDist);

        int drawStart = -lineHeight / 2 + mWindow.GetHeight() / 2;

        int texNum = pMap[mapY * pMap.GetHeight() + mapX] - 1;

        if(texNum < 0)
            continue;

        const Texture* wallTexture = textures[texNum];

        double wallX;

        if(side == 0)
            wallX = ppos.y + perpWallDist * rayDir.y;
        else
            wallX = ppos.x + perpWallDist * rayDir.x;


        wallX -= floor(wallX);

        int texX = (int)(wallX * (double)wallTexture->GetWidth());
        if(side == 0 && rayDir.x > 0) texX = wallTexture->GetWidth() - texX - 1;
        if(side == 1 && rayDir.y < 0) texX = wallTexture->GetWidth() - texX - 1;

        SDL_Rect src = {texX, 0, 1, wallTexture->GetHeight()};
        SDL_Rect dst = {x, drawStart, 1, lineHeight};

        RenderTexture(*wallTexture, src, dst);
    }
}

void CastEngine::Renderer::ClearScreen(SDL_Color &colour)
{
    SDL_Renderer* render = mWindow.GetRenderer();
    ResetDepthBuffer();

    if(SDL_SetRenderDrawColor(render, colour.r, colour.g, colour.b, colour.a) < 0)
    {
        LogMsgf(ERROR, "failed to clear screen. SDL_ERROR: %s", SDL_GetError());
        return;
    }
    
    if(SDL_RenderClear(render) < 0)
        LogMsgf(ERROR, "failed to clear screen. SDL_ERROR: %s", SDL_GetError());

}

void CastEngine::Renderer::Present()
{
    SDL_RenderPresent(mWindow.GetRenderer());
}

void CastEngine::Renderer::Destroy()
{
    texBank.FreeAll();
}
