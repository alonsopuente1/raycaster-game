#include "t_textures.h"

#include <SDL2/SDL_image.h>

#include "settings.h"
#include "w_window.h"

#include "logger.h"

extern window_t gMainWindow;

texture_t* gTextures;

extern int NUMTEXTURES;

void fileNameFromPath(const char* path, char* outName, int maxLen)
{
    char* lastSlash = (char*)path;
    char* lastDot = (char*)path;
    for(char* p = (char*)path; *p != '\0'; p++)
    {
        if(*p == '/' || *p == '\\')
            lastSlash = p + 1;
        if(*p == '.')
            lastDot = p;
    }

    int charsToCopy = lastDot - lastSlash;
    if(charsToCopy >= maxLen)
        charsToCopy = maxLen - 1;
    memcpy(outName, lastSlash, charsToCopy);
    outName[charsToCopy] = '\0';
}

texture_t T_LoadTexture(const char* path)
{
    texture_t tex;
    tex.data = IMG_LoadTexture(gMainWindow.sdlRenderer, path);
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

texture_t* T_FindTexture(const char* texName)
{
    for(int i = 0; i < NUMTEXTURES; i++)
        if(strcmp(gTextures[i].name, texName) == 0)
            return &gTextures[i];

    return NULL;
}

texture_t T_CreateBlankTexture(const char* name, int width, int height)
{
    texture_t tex = {0};
    tex.width = width;
    tex.height = height;
    tex.data = SDL_CreateTexture(gMainWindow.sdlRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
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

void T_FreeTexture(texture_t* tex)
{
    if(tex->data)
        SDL_DestroyTexture(tex->data);
    tex->data = NULL;
    tex->width = 0;
    tex->height = 0;
}