#include "w_buttons.h"

#include "logger.h"
#include "fonts.h"
#include "m_game.h"
#include "r_renderer.h"

winButton_t W_CreateButton(renderer_t* render, SDL_Rect rect, SDL_Color colour)
{
    winButton_t output = { 0 };
    if(!render)
    {
        LogMsg(WARN, "passed null ptr to renderer\n");
        return output;
    }

    output.rect = rect;
    output.backgroundColor = colour;

    output.parentRenderer = render;

    return output;
}

void W_SetButtonText(winButton_t* button, const char* src)
{
    if(!button)
    {
        LogMsg(WARN, "passed null ptr to button\n");
        return;
    }

    if(!button->parentRenderer)
    {
        LogMsg(WARN, "trying to set button text without parent renderer set\n");
    }

    SDL_Color white = (SDL_Color){0xff, 0xff, 0xff, 0xff};

    if(button->text)
        if(!TB_RemoveTextureByPtr(&button->parentRenderer->textureBank, button->text))
        {
            LogMsg(ERROR, "MEMORY LEAK failed to free old text\n");
            return;
        }

    button->text = F_CreateText(button->parentRenderer, white, fonts[0], src);


    if(!button->text)
    {
        LogMsg(WARN, "failed to set button texture");
        return;
    }
}

void W_SetButtonRect(winButton_t* button, SDL_Rect src)
{
    if(!button)
    {
        LogMsg(WARN, "passed null ptr to button\n");
        return;
    }

    button->rect = src;
}

void W_DrawButton(winButton_t* button)
{
    if(!button)
    {
        LogMsg(WARN, "passing null ptr to button\n");
        return;
    }

    SDL_Renderer*   dstSdlRenderer = button->parentRenderer->parentWindow->sdlRenderer;

    SDL_SetRenderDrawBlendMode(dstSdlRenderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(dstSdlRenderer, 
        button->backgroundColor.r,
        button->backgroundColor.g,
        button->backgroundColor.b,
        button->backgroundColor.a
    );

    SDL_RenderFillRect(dstSdlRenderer, &button->rect);
    if(button->text)
        if(SDL_RenderCopy(dstSdlRenderer, button->text->data, NULL, &button->rect) < 0)
            LogMsgf(WARN, "failed to render text to button. SDL_ERROR: %s\n", SDL_GetError());
    SDL_SetRenderDrawBlendMode(dstSdlRenderer, SDL_BLENDMODE_NONE);
}


void W_DestroyButton(winButton_t* button)
{
    if(!button)
    {
        LogMsg(WARN, "passed null ptr to button\n");
        return;
    }

    if(button->text)
        if(!TB_RemoveTextureByPtr(&button->parentRenderer->textureBank, button->text))
            LogMsg(ERROR, "MEMORY LEAK failed to free button text texture\n");

    button->backgroundColor = (SDL_Color){0,0,0,0};
    button->parentRenderer = NULL;
    button->rect = (SDL_Rect){0,0,0,0};
    button->text = NULL;
}