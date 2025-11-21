#include "r_renderer.h"

#include <float.h>

#include "w_window.h"

#include "logger.h"

renderer_t R_CreateRenderer(window_t* parentWindow)
{
    if(!parentWindow)
    {
        LogMsg(ERROR, "passed null ptr to parentWindow\n");
        return (renderer_t) { 0 };
    }

    renderer_t output = { 0 };

    output.parentWindow = parentWindow;

    output.depthBuffer = calloc(parentWindow->width, sizeof(float));
    if(!output.depthBuffer)
    {
        LogMsg(ERROR, "could not allocate memory for the depth buffer\n");
        return (renderer_t) { 0 };
    }
    
    for(int i = 0; i < parentWindow->width; i++)
        output.depthBuffer[i] = FLT_MAX;

    return output;
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