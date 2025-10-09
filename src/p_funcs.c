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
    if(p->moveState & 0b1) // Player moving forward
    {
        P_Move(p, p->moveSpeed * dt);
    }
    if(p->moveState & 0b10) // Player moving backwards
    {
        P_Move(p, -p->moveSpeed * dt);
    }
    if(p->moveState & 0b100) // Player strafing left
    {
        P_Strafe(p, p->moveSpeed * dt);
    }
    if(p->moveState & 0b1000) // Player strafing right
    {
        P_Strafe(p, -p->moveSpeed * dt);
    }
    if(p->moveState & 0b10000) // Player rotating right
    {
        P_Rotate(p, p->rotateSpeed * dt);
    }
    if(p->moveState & 0b100000) // Player rotating left
    {
        P_Rotate(p, -p->rotateSpeed * dt);
    }

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