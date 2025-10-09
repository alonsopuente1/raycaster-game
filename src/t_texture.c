#include "t_textures.h"

#include "logger.h"
#include <SDL2/SDL_image.h>

extern SDL_Renderer* gRenderer;

texture_t T_LoadTexture(const char* path)
{
    texture_t tex;
    tex.data = IMG_LoadTexture(gRenderer, path);
    if(!tex.data)
    {
        LogMsgf(ERROR, "Failed to load texture at path '%s'. IMG_ERROR: %s", path, IMG_GetError());
        tex.width = 0;
        tex.height = 0;
        return tex;
    }
    SDL_QueryTexture(tex.data, NULL, NULL, &tex.width, &tex.height);
    return tex;
}