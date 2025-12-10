#include "texture.h"

#include <SDL2/SDL_image.h>

#include "settings.h"
#include "w_window.h"
#include "util.h"
#include "logger.h"

texture_t T_LoadTexture(window_t* window, const char* path)
{
    texture_t tex;
    tex.data = IMG_LoadTexture(window->sdlRenderer, path);
    if(!tex.data)
    {
        LogMsgf(ERROR, "Failed to load texture at path '%s'. IMG_ERROR: %s", path, IMG_GetError());
        tex.width = 0;
        tex.height = 0;
        tex.data = NULL;
        return tex;
    }

    fileNameFromPath(path, tex.name, sizeof(tex.name));
    SDL_QueryTexture(tex.data, NULL, NULL, &tex.width, &tex.height);

    return tex;
}

texture_t T_CreateBlankTexture(window_t* window, const char* name, int width, int height)
{
    texture_t tex = {0};
    tex.width = width;
    tex.height = height;
    tex.data = SDL_CreateTexture(window->sdlRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    if(!tex.data)
    {
        LogMsgf(ERROR, "Failed to create blank texture of size %d x %d. SDL_ERROR: %s\n", width, height, SDL_GetError());
        tex = (texture_t){0};
        return tex;
    }
    strncpy(tex.name, name, 32);
    tex.name[sizeof(tex.name) - 1] = '\0';
    return tex;
}

bool T_SetTextureName(texture_t* tex, const char* name)
{
    if(!tex)
    {
        LogMsg(ERROR, "passed null ptr to tex\n");
        return false;
    }

    strncpy(tex->name, name, sizeof(tex->name) - 1);
    int nullTermIndex = min(strlen(name), sizeof(tex->name) - 1);
    tex->name[nullTermIndex] = '\0';

    return true;
}

void T_FreeTexture(texture_t* tex)
{
    if(!tex)
        return;
        
    if(tex->data)
        SDL_DestroyTexture(tex->data);
    tex->data = NULL;
    tex->width = 0;
    tex->height = 0;
}