#include "w_buttons.h"

#include "logger.h"
#include "fonts.h"
#include "m_game.h"

winButton_t W_CreateButton(maingame_t* game, SDL_Rect rect, SDL_Color colour)
{
    winButton_t output = { 0 };
    if(!game)
    {
        LogMsg(WARN, "passed null ptr to game\n");
        return output;
    }

    output.rect = rect;
    output.backgroundColor = colour;

    memset(&output.text, 0, sizeof(output.text));

    output.parentGame = game;

    return output;
}

void W_SetButtonText(winButton_t* button, const char* src)
{
    if(!button)
    {
        LogMsg(WARN, "passed null ptr to button\n");
        return;
    }

    if(!button->parentGame)
    {
        LogMsg(WARN, "trying to set button text without parent game set\n");
    }

    SDL_Color white = (SDL_Color){0xff, 0xff, 0xff, 0xff};

    if(button->text)
        if(!TB_RemoveTextureByPtr(&button->parentGame->texturebank, button->text))
        {
            LogMsg(ERROR, "MEMORY LEAK failed to free old text\n");
            return;
        }

    button->text = F_CreateText(button->parentGame, white, fonts[0], src);


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

    SDL_SetRenderDrawBlendMode(button->parentGame->window.sdlRenderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(button->parentGame->window.sdlRenderer, 
        button->backgroundColor.r,
        button->backgroundColor.g,
        button->backgroundColor.b,
        button->backgroundColor.a
    );

    SDL_RenderFillRect(button->parentGame->window.sdlRenderer, &button->rect);
    if(button->text)
        if(SDL_RenderCopy(button->parentGame->window.sdlRenderer, button->text->data, NULL, &button->rect) < 0)
            LogMsgf(WARN, "failed to render text to button. SDL_ERROR: %s\n", SDL_GetError());
    SDL_SetRenderDrawBlendMode(button->parentGame->window.sdlRenderer, SDL_BLENDMODE_NONE);
}