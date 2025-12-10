#include "r_renderer.h"

#include <float.h>

#include "w_window.h"

#include "logger.h"

/* FORWARD DECLARATIONS */

void R_ResetDepthBuffer(renderer_t* render);

/* PUBLIC FUNCTIONS*/
renderer_t R_CreateRenderer(window_t* parentWindow)
{
    if(!parentWindow)
    {
        LogMsg(ERROR, "passed null ptr to parentWindow\n");
        return (renderer_t) { 0 };
    }

    renderer_t output = { 0 };

    output.parentWindow = parentWindow;
    output.depthBufferCount = parentWindow->width;

    output.depthBuffer = calloc(output.depthBufferCount, sizeof(float));
    if(!output.depthBuffer)
    {
        LogMsg(ERROR, "could not allocate memory for the depth buffer\n");
        return (renderer_t) { 0 };
    }
    
    for(int i = 0; i < output.depthBufferCount; i++)
        output.depthBuffer[i] = FLT_MAX;

    return output;
}

void R_ClearScreen(renderer_t* render, SDL_Color colour)
{
    if(!render)
    {
        LogMsg(WARN, "passed null ptr to renderer\n");
        return;
    }

    SDL_Renderer* sdlRenderer = render->parentWindow->sdlRenderer;

    R_ResetDepthBuffer(render);

    SDL_SetRenderDrawColor(sdlRenderer, colour.r, colour.g, colour.b, colour.a);
    SDL_RenderClear(sdlRenderer);
}

void R_Present(renderer_t* render)
{
    if(!render)
    {
        LogMsg(WARN, "passed null ptr to renderer\n");
        return;
    }

    SDL_RenderPresent(render->parentWindow->sdlRenderer);
}

void R_DestroyRenderer(renderer_t* render)
{
    if(!render)
    {
        LogMsg(WARN, "passed null ptr to render\n");
        return;
    }

    if(render->depthBuffer)
    {
        free(render->depthBuffer);
        render->depthBuffer = NULL;
    }

    render->parentWindow = NULL;

    TB_FreeAllTextures(&render->textureBank);
}

/* PRIVATE FUNCTIONS */

// since its a private function, render and render->depthBuffer 
// are guaranteed to be non-null
void R_ResetDepthBuffer(renderer_t* render)
{
    for(int i = 0; i < render->depthBufferCount; i++)
        render->depthBuffer[i] = FLT_MAX;
}