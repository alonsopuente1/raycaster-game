#include "player.hpp"

#include "logger.hpp"
#include "map.hpp"

namespace CastEngine
{
    Player::Player(Map &pMap) : mAttachedMap(pMap),
        mAcc(0.f), mVel(0.f), mPos(0.0f), mHitbox(0.0f),
        mFov(DegToRad(90.0f))
    {}

    void Player::HandleKeyInput(const SDL_Event &e)
    {
        if(e.type != SDL_KEYDOWN && e.type != SDL_KEYUP)
        {
            LogMsg(ERROR, "passed wrong event type to player");
            return;
        }

        // 1 if key up, 0 if down
        int uporDown = SDL_KEYUP - SDL_KEYDOWN;
        bool state = true;
        if(uporDown)
        {
            state = false;
        }


        // TODO: make a menu to be able to change
        // controls
        if(e.key.keysym.sym == SDLK_w)
        {
            
        }

        switch(e.key.keysym.sym)
        {
            case SDLK_w:
                State.forward = state;
                break;
            case SDLK_s:
                State.backward = state;
                break;
            case SDLK_a:
                State.left = state;
                break;
            case SDLK_d:
                State.right = state;
                break;
        }
    }

    void Player::Update(float dtMs)
    {
        vec2d newAcc(0.0f);
        vec2d viewDir(vec2d::AngToVec(mViewAng));
        vec2d oldPos = mPos;

        if(State.forward)
        {
            newAcc += viewDir;
        }   
        if(State.backward)
        {
            newAcc -= viewDir;
        }
        if(State.left)
        {
            newAcc += viewDir.GetPerpendicular();
        }
        if(State.right)
        {
            newAcc -= viewDir.GetPerpendicular();
        }
        if(State.lookleft)
        {
            Rotate(mRotateSpeed * dtMs);
        }
        if(State.lookright)
        {
            Rotate(mRotateSpeed * dtMs * -1.f);
        }
        
        // if player not moving apply friction
        if(!newAcc.GetMagnitude())
        {
            newAcc = mVel * -0.05f;
        }
        else
        {
            newAcc.SetMagnitude(0.0005f);
        }

        // update velocity
        mVel += newAcc;

        // clamp velocity magnitude
        if(mVel.GetMagnitude() > mMaxMoveSpeed)
            mVel.SetMagnitude(mMaxMoveSpeed);
        else if(mVel.GetMagnitude() < 0.000001f);
            mVel = vec2d(0.0f);

        mPos += mVel * dtMs;
        vec2d deltaPos = mPos - oldPos;
        
        if(mAttachedMap[(int) mPos.y * mAttachedMap.GetWidth() + (int)(oldPos.x + deltaPos.x)] > 0)
            mPos.x -= deltaPos.x;
        if(mAttachedMap[(int)(oldPos.y + deltaPos.y) * mAttachedMap.GetWidth() + (int)(oldPos.x)] > 0)
            mPos.y -= deltaPos.y;
    }

    void Player::Walk(float distance)
    {
        
    }
    void Player::Rotate(float ang)
    {
        mViewAng += ang;
    }
}