#include "t_textures.h"

#include "logger.h"
#include <SDL2/SDL_image.h>
#include "settings.h"

extern SDL_Renderer* gRenderer;

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
    tex.data = IMG_LoadTexture(gRenderer, path);
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
    {
        if(strcmp(gTextures[i].name, texName) == 0)
            return &gTextures[i];
    }

    return NULL;
}

void T_FreeTexture(texture_t* tex)
{
    if(tex->data)
        SDL_DestroyTexture(tex->data);
    tex->data = NULL;
    tex->width = 0;
    tex->height = 0;
}