#include "fonts.h"

#include "logger.h"

// array of all fonts to load
const char* fontFilePath[] = {
    "res/fonts/runescape.ttf"
};

TTF_Font* fonts[64] = { 0 }; 

void FontInit()
{
    int numOfFilePaths = sizeof(fontFilePath) / sizeof(const char*);

    int iterations = numOfFilePaths > 64 ? 64 : numOfFilePaths;

    for(int i = 0; i < iterations; i++)
    {
        fonts[i] = TTF_OpenFont(fontFilePath[i], 16);
        if(!fonts[i])
            LogMsgf(WARN, "failed to load font at filepath '%s'. TTF_ERROR: %s\n", fontFilePath[i], TTF_GetError());
    }
}

texture_t* F_CreateText(maingame_t* dst, SDL_Color colour, TTF_Font* font, const char* text)
{
    if(!dst)
    {
        LogMsg(WARN, "passed null ptr to game\n");
        return NULL;
    }

    texture_t*  output = TB_AddEmptyTexture(&dst->texturebank);

    if(!output)
    {
        LogMsg(ERROR, "failed to add empty texture to game texturebank\n");
        return NULL;
    }

    SDL_Texture* texture = NULL;
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, colour);
    if(!surface)
    {
        LogMsgf(ERROR, "failed to render text to surface. TTF_ERROR: %s\n", TTF_GetError());
        if(TB_RemoveTextureByPtr(&dst->texturebank, output))
            LogMsg(ERROR, "MEMORY LEAK failed to remove texture from the texturebank\n");
        return NULL;
    }

    texture = SDL_CreateTextureFromSurface(dst->window.sdlRenderer, surface);
    if(!texture)
    {
        LogMsgf(ERROR, "failed to create texture from surface. SDL_ERROR: %s\n", SDL_GetError());
        if(TB_RemoveTextureByPtr(&dst->texturebank, output))
            LogMsg(ERROR, "MEMORY LEAK failed to remove texture from the texturebank\n");
        SDL_FreeSurface(surface);
        return NULL;
    }

    SDL_FreeSurface(surface);

    int w, h;
    if(SDL_QueryTexture(texture, NULL, NULL, &w, &h) < 0)
    {    
        LogMsgf(WARN, "failed to get width and height for text: %s\n", text);
    }
    else
    {
        output->width = w;
        output->height = h;
    }

    output->data = texture;

    return output;
}

void FontCleanup()
{
    for(int i = 0; i < sizeof(fonts) / sizeof(TTF_Font*); i++)
        TTF_CloseFont(fonts[i]);
}