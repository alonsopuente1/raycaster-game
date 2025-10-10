#include "t_textures.h"

#include "logger.h"
#include <SDL2/SDL_image.h>
#include "settings.h"

extern SDL_Renderer* gRenderer;

texture_t* gTextures;

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
    SDL_QueryTexture(tex.data, NULL, NULL, &tex.width, &tex.height);
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