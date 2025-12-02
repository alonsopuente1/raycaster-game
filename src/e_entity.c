#include "e_entity.h"

#include "v_funcs.h"
#include "r_renderer.h"
#include "p_player.h"
#include "map.h"

#include <SDL2/SDL.h>

#include "logger.h"

// sprite drawing algorithm mostly taken from https://lodev.org/cgtutor/raycasting3.html
// with slight alterations to work with this project
void E_DrawEntity(renderer_t* render, player_t* player, entity_t* e)
{
    if(!e)
    {
        LogMsg(WARN, "passed null ptr to entity\n");
        return;
    }

    if(!e->active)
        return;

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

    SDL_Renderer* sdlRenderer = render->parentWindow->sdlRenderer;

    vertex2d_t playerToEntity = V_Sub(e->pos, player->pos);
    
    vertex2d_t playerDir = V_AngToVec(player->viewAngle);
    vertex2d_t plane = V_GetPerpendicular(playerDir);
    texture_t* tex = e->entityTex;
    V_SetMagnitude(&plane, -tanf(player->fov / 2.0f));

    float invDet = 1.0f / (plane.x * playerDir.y - playerDir.x * plane.y);

    vertex2d_t transform = {
        invDet * (playerDir.y * playerToEntity.x - playerDir.x * playerToEntity.y),
        invDet * (-plane.y * playerToEntity.x + plane.x * playerToEntity.y)
    };

    int spriteScreenX = (int)((render->parentWindow->width / 2.f) * (1.f + transform.x / transform.y));

    int spriteHeight = abs((int)(render->parentWindow->height / transform.y));

    int spriteWidth = (tex->width / tex->height) * spriteHeight;

    int drawStartX = -spriteWidth / 2.f + spriteScreenX;
    int drawEndX = spriteWidth / 2.f + spriteScreenX;

    SDL_SetRenderDrawBlendMode(sdlRenderer, SDL_BLENDMODE_BLEND);
    for(int i = drawStartX; i < drawEndX; i++)
    {
        int texX = (int)((float)(i - drawStartX) / (float)(drawEndX - drawStartX) * tex->width);
        
        SDL_Rect src = {texX, 0, 1, tex->height};
        SDL_Rect dst = {i, -spriteHeight / 2 + render->parentWindow->height / 2.f, 1, spriteHeight};

        if(transform.y > 0 && i > 0 && i < render->parentWindow->width && transform.y < render->depthBuffer[i])
        {
            render->depthBuffer[i] = transform.y;
            SDL_RenderCopy(sdlRenderer, tex->data, &src, &dst);
        }
    }
    SDL_SetRenderDrawBlendMode(sdlRenderer, SDL_BLENDMODE_NONE);
}

bool E_IsEqual(entity_t a, entity_t b)
{
    return memcmp(&a, &b, sizeof(entity_t)) == 0;
}

void E_Update(entity_t* e, float deltaTime, map_t* m)
{
    if(!e)
    {
        LogMsg(WARN, "passed null ptr to entity\n");
        return;
    }

    if(!e->active)
        return;

    if(e->think)
        e->think(e, deltaTime);

    e->vel = V_Add(e->vel, V_Mul(e->acc, deltaTime));
    if(V_GetMagnitude(e->vel) > e->maxSpeed)
        V_SetMagnitude(&e->vel, e->maxSpeed);

    e->pos = V_Add(e->pos, V_Mul(e->vel, deltaTime));

    // simple collision with walls
    if(m)
    {
        // does adding x put entity in wall?
        if(M_GetMapCell(m, (int)e->pos.y * m->mapWidth + (int)(e->pos.x)) > 0)
            e->pos.x -= e->vel.x * deltaTime;
        // does adding y put entity in wall?
        if(M_GetMapCell(m, (int)(e->pos.y) * m->mapWidth + (int)(e->pos.x)) > 0)
            e->pos.y -= e->vel.y * deltaTime;
    }
}