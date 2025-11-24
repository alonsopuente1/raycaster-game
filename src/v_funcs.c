#include "v_funcs.h"

vertex2d_t V_Make(const float a, const float b)
{
    vertex2d_t out = {a, b};
    return out;
}

vertex2d_t V_Add(const vertex2d_t a, const vertex2d_t b)
{
    vertex2d_t out = {a.x + b.x, a.y + b.y};
    return out;
}

vertex2d_t V_Sub(const vertex2d_t a, const vertex2d_t b)
{
    vertex2d_t out = {a.x - b.x, a.y - b.y};
    return out;
}

vertex2d_t V_Mul(const vertex2d_t a, float mag)
{
    vertex2d_t out = {a.x * mag, a.y * mag};
    return out;
}

vertex2d_t V_Div(const vertex2d_t a, float mag)
{
    vertex2d_t out = {a.x / mag, a.y / mag};
    return out;
}

float V_DotProduct(const vertex2d_t a, const vertex2d_t b)
{
    return (a.x * b.x) + (a.y * b.y);
}

float V_GetAngBetweenVecs(const vertex2d_t a, const vertex2d_t b)
{
    return acosf(V_DotProduct(a, b) / (V_GetMagnitude(a) * V_GetMagnitude(b)));
}

bool V_IsEqual(vertex2d_t a, vertex2d_t b)
{
    return (a.x == b.x && a.y == b.y);
}

vertex2d_t V_GetPerpendicular(const vertex2d_t a)
{
    vertex2d_t out = {a.y, -a.x};
    return out;
}

vertex2d_t V_AngToVec(const float angle)
{
    vertex2d_t out = {cos(angle), sin(angle)};
    return out;
}

float V_GetMagnitude(const vertex2d_t a)
{
    return (float)sqrt((double)a.x * (double)a.x + (double)a.y * (double)a.y);
}

vertex2d_t V_Normalise(const vertex2d_t a)
{
    vertex2d_t out = V_Div(a, V_GetMagnitude(a));
    return out;
}

vertex2d_t V_Rotate(const vertex2d_t a, const float angle)
{
    vertex2d_t out = {a.x * cos(angle) - a.y * sin(angle), a.x * sin(angle) + a.y * cos(angle)};
    return out;
}

void V_SetMagnitude(vertex2d_t* a, const float mag)
{
    if(V_GetMagnitude(*a) == 0.f)
        return;
    *a = V_Mul(*a, mag / V_GetMagnitude(*a));
}