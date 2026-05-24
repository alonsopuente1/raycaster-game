#include "animatedtexture.h"

#include "texturebank.h"

#include "logger.h"

void T_UpdateAnimatedTexture(animatedTexture_t* animTex, float dtMs)
{
    if(!animTex)
    {
        LogMsg(WARN, "passed null ptr to animated texture\n");
        return;
    }

    if(!(animTex->playAnimation || animTex->frameIndex > 0))
        return;

    if(animTex->numFrames <= 0)
    {
        LogMsg(WARN, "animated texture has no frames\n");
        return;
    }


    animTex->currentFrameTime += dtMs;

    if(animTex->currentFrameTime > animTex->frameTime)
    {
        animTex->frameIndex = (animTex->frameIndex + 1) % animTex->numFrames;
        while(animTex->currentFrameTime > animTex->frameTime)
            animTex->currentFrameTime -= animTex->frameTime;
    }
}

bool T_PushFrame(animatedTexture_t* animTex, texture_t* tex)
{
    if(!animTex)
    {
        LogMsg(WARN, "passed null ptr to animated texture\n");
        return false;
    }

    if(animTex->numFrames >= ANIMTEX_MAXFRAMES)
    {
        LogMsg(WARN, "trying to add frame to full animated texture");
        return false;
    }

    if(!T_TextureInitialised(tex))
    {
        LogMsg(WARN, "texture not initialised");
        return false;
    }

    animTex->frames[animTex->numFrames++] = tex;

    return true;
}

void T_PlayAnimation(animatedTexture_t* animTex)
{
    if(!animTex)
    {
        LogMsg(WARN, "passed null ptr to animated textures");
        return;
    }

    animTex->playAnimation = true;
}

void T_PlayAnimationOnce(animatedTexture_t* animTex)
{
    if(!animTex)
    {
        LogMsg(WARN, "passed null ptr to animated texture");
        return;
    }

    animTex->frameIndex = 1;
}

void T_StopAnimation(animatedTexture_t* animTex)
{
    if(!animTex)
    {
        LogMsg(WARN, "passed null ptr to animated textures");
        return;
    }

    animTex->playAnimation = false;
}

void T_ResetAnimation(animatedTexture_t* animTex)
{
    if(!animTex)
    {
        LogMsg(WARN, "passed null ptr to animated textures");
        return;
    }

    animTex->frameIndex = 0;
    animTex->currentFrameTime = 0;
}

int T_NumberFrames(animatedTexture_t* animTex)
{
    if(!animTex)
    {
        LogMsg(WARN, "passed null ptr to animated textures");
        return -1;
    }

    int count;
    for(count = 0; count < ANIMTEX_MAXFRAMES; count++)
        if(!animTex->frames[count])
            break;

    return count;
}

texture_t* T_GetCurrentFrame(animatedTexture_t* animTex)
{
    if(!animTex)
    {
        LogMsg(WARN, "passed null ptr to animated textures");
        return NULL;
    }

    return animTex->frames[animTex->frameIndex];
}
