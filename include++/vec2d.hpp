#pragma once

#include <cmath>

struct vec2d
{
    float x, y;

    // normal constructors

    vec2d();
    vec2d(float _x, float _y);

    // copy/move assignment/constructors

    vec2d& operator=(const vec2d& other);

    vec2d(const vec2d& other);
    vec2d(vec2d&& other);

    vec2d& operator=(vec2d&& other);

    // math operations

    vec2d operator+(const vec2d& other);

    vec2d& operator+=(const vec2d& other);

    vec2d operator-(const vec2d& other);

    vec2d& operator-=(const vec2d& other);

    vec2d operator*(float mag);

    vec2d& operator*=(float mag);

    vec2d operator/(float mag);

    vec2d operator/=(float mag);

    // equality operations

    bool operator==(const vec2d& other);

    bool operator!=(const vec2d& other);

    // public helper methods

    float GetMagnitude() const;

    void SetMagnitude(float mag);
    
    vec2d GetPerpendicular() const;

    static vec2d AngToVec(float angle);

    void Normalise();

    void Rotate(float angle);

    float DotProduct(const vec2d& other);


};

inline float DegToRad(float deg)
{
    return deg * (acosf(-1) / 180.f);
}

inline float RadToDeg(float rad)
{
    return rad * (180.f / acosf(-1));
}