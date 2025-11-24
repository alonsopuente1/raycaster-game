#ifndef __V_FUNCS_H__
#define __V_FUNCS_H__

#include "v_vert.h"

#include <stdbool.h>

extern vertex2d_t V_Make(const float a, const float b);
extern vertex2d_t V_Add(const vertex2d_t a, const vertex2d_t b);
extern vertex2d_t V_Sub(const vertex2d_t a, const vertex2d_t b);
extern vertex2d_t V_Mul(const vertex2d_t a, float mag);
extern vertex2d_t V_Div(const vertex2d_t a, float mag);
extern float      V_DotProduct(const vertex2d_t a, const vertex2d_t b);
// returns angle in radians
extern float      V_GetAngBetweenVecs(const vertex2d_t a, const vertex2d_t b);

extern bool       V_IsEqual(vertex2d_t a, vertex2d_t b);

// gets the perpendicular vector to 'a' 90 degrees anti-clockwise
extern vertex2d_t V_GetPerpendicular(const vertex2d_t a);

// angle in radians
extern vertex2d_t V_AngToVec(const float angle);
extern 		float V_GetMagnitude(const vertex2d_t a);
extern 		 void V_SetMagnitude(vertex2d_t* a, const float mag);
extern vertex2d_t V_Normalise(const vertex2d_t a);

// angle in radians
extern vertex2d_t V_Rotate(const vertex2d_t a, const float angle);

#endif