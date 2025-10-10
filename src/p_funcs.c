#include "p_funcs.h"
#include "v_funcs.h"
#include "m_map.h"

void P_Move(player_t* p, const float speed)
{
    vertex2d_t viewVec = {cos(p->viewAngle), sin(p->viewAngle)};
    viewVec = V_Mul(viewVec, speed);

    p->pos = V_Add(p->pos, viewVec);
}

void P_Strafe(player_t* p, const float speed)
{
    vertex2d_t perpVec = V_GetPerpendicular(V_AngToVec(p->viewAngle));
    p->pos = V_Add(p->pos, V_Mul(perpVec, speed));
}

void P_Rotate(player_t* p, const float ang)
{
    p->viewAngle += ang;
}

void P_HandleState(player_t* p, map_t* m, float dt)
{
    vertex2d_t oldPos = p->pos;
    vertex2d_t viewDir = V_Mul(V_AngToVec(p->viewAngle), 0.001f);
    vertex2d_t newAcc = {0.f, 0.f};

    if(p->moveState & MOVE_FORWARD) // Player moving forward
        newAcc = V_Add(newAcc, viewDir);
    if(p->moveState & MOVE_BACKWARD) // Player moving backwards
        newAcc = V_Add(newAcc, V_Mul(viewDir, -1.f));
    if(p->moveState & STRAFE_LEFT) // Player strafing left
        newAcc = V_Add(newAcc, V_GetPerpendicular(viewDir));
    if(p->moveState & STRAFE_RIGHT) // Player strafing right
        newAcc = V_Add(newAcc, V_GetPerpendicular(V_Mul(viewDir, -1.f)));
    if(p->moveState & ROTATE_RIGHT) // Player rotating right
        P_Rotate(p, p->rotateSpeed * dt);
    if(p->moveState & ROTATE_LEFT) // Player rotating left
        P_Rotate(p, -p->rotateSpeed * dt);
    
    if(!V_GetMagnitude(newAcc))
    {
        newAcc = V_Mul(p->vel, -0.2f); // apply friction
    }
    
    
    p->vel = V_Add(p->vel, newAcc);
    if(V_GetMagnitude(p->vel) > p->maxMoveSpeed)
        V_SetMagnitude(&p->vel, p->maxMoveSpeed);
    else if(V_GetMagnitude(p->vel) < 0.000001f)
    {    
        p->vel = (vertex2d_t){0.f, 0.f};
        return;
    }

    p->gunSway += V_GetMagnitude(p->vel) * dt;
    while(p->gunSway > acosf(-1) * 2)
        p->gunSway -= acosf(-1) * 2;

    p->pos = V_Add(p->pos, V_Mul(p->vel, dt));
    vertex2d_t deltaPos = V_Sub(p->pos, oldPos);

    if(!m)
        return;
    
    if(m->mapData[(int)p->pos.y * m->mapWidth + (int)p->pos.x > 0])
    {
        // does adding x put player in wall?
        if(m->mapData[(int)oldPos.y * m->mapWidth + (int)(oldPos.x + deltaPos.x)])
        {
            p->pos.x -= deltaPos.x;
        }

        // does adding y put player in wall?
        if(m->mapData[(int)(oldPos.y + deltaPos.y) * m->mapWidth + (int)(oldPos.x)])
        {
            p->pos.y -= deltaPos.y;
        }
    }
}

void P_RenderCurrentGun(player_t* p)
{
    switch(p->currentGun)
    {
        case FISTS:
            break;
        case PISTOL:
            break;
        case SHOTGUN:
            break;
        default:
            break;
    }
}