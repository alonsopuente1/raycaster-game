#ifndef __ANIMATEDTEXTURE_H__
#define __ANIMATEDTEXTURE_H__

#include "texture.h"


// maximum frames an animated texture can have
#define ANIMTEX_MAXFRAMES 16

typedef struct animatedTexture_s 
{

    // array of pointers to textures that make up the animation frames
    // should be ordered in the sequence they are meant to be played in
    // all frames should be 
    texture_t*  frames[ANIMTEX_MAXFRAMES];

    // number of frames in the animation
    int         numFrames;
    // index into the frames array for current frame to render
    int         frameIndex;
    
    // time in milliseconds each frame should be displayed for
    float       frameTime;
    // time the current frame has been displayed for 
    float       currentFrameTime; 

    bool        playAnimation;

} animatedTexture_t;

// should be called once a frame in the update method of scene
extern void T_UpdateAnimatedTexture(animatedTexture_t* animTex, float dtMs);

// adds a frame to the end of the array
extern bool T_PushFrame(animatedTexture_t* animTex, texture_t* tex);

// plays animation until its stopped again
extern void T_PlayAnimation(animatedTexture_t* animTex);

// plays animation just a single time
extern void T_PlayAnimationOnce(animatedTexture_t* animTex);

// stops the animation playing
extern void T_StopAnimation(animatedTexture_t* animTex);

// puts frame onto first again and resets timer
// doesn't change whether animation is playing
extern void T_ResetAnimation(animatedTexture_t* animTex);

extern int T_NumberFrames(animatedTexture_t* animTex);

// returns pointer to current frame
extern texture_t* T_GetCurrentFrame(animatedTexture_t* animTex);

#endif