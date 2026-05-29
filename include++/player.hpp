#pragma once

#include "vec2d.hpp"
#include <SDL2/SDL.h>

namespace CastEngine
{
    class Map;
    class Player
    {

    private:
        
        /// @brief stored in radians
        float mViewAng;

        float mMaxMoveSpeed;
        
        /// @brief in radians
        float mFov;

        vec2d mAcc;
        vec2d mVel;
        vec2d mPos;

        vec2d mHitbox;

        float mRotateSpeed;

        /// @brief stores the state of the player
        struct 
        {
            bool forward : 1;
            bool backward : 1;
            bool left : 1;
            bool right : 1;
            bool lookleft : 1;
            bool lookright : 1;
            bool shooting : 1;
        } State;


        Map& mAttachedMap;
        
        /// @brief walks the player forwards or backwards
        /// @param distance positive value for forwards, negative for backwards
        void Walk(float distance);
        
        void Strafe(float distance);
        
        void Rotate(float ang);

    public:

        Player(Map& pMap);

        inline vec2d GetAcc() const { return mAcc; }
        inline vec2d GetVel() const { return mVel; }
        inline vec2d GetPos() const { return mPos; }

        inline float GetViewAng() const { return mViewAng; }

        void HandleKeyInput(const SDL_Event& e);
        void Update(float dtMs);
    };
};