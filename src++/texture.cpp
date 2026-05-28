#include "texture.hpp"

#include <SDL2/SDL_image.h>

#include "logger.h"

#include "window.hpp"

namespace CastEngine
{
 
    #define CHECKTEX(texture) if(texture == NULL) { LogMsg(WARN, "passed null ptr to texture"); return;}

    Texture::Texture(Window& window) : mWindow(window)
    {}
    
    Texture::Texture(Window& window, const std::string& file) : mWindow(window)
    {

    }
    
    Texture::Texture(Window& window, const std::string& name, int width, int height) : mWindow(window)
    {

    }
    
    Texture::~Texture()
    {
        Destroy();
    }
    
    uint32_t Texture::GetWidth() const
    {


        int width;
        if(SDL_QueryTexture(mSDLTex, NULL, NULL, &width, NULL) < 0)
        {
            LogMsg(ERROR, "failed to retrieve width of texture (%s)", mName);
            return 0;
        }
        return width;
    }
    uint32_t Texture::GetHeight() const
    {
        return mHeight;
    }
    
    void Texture::Destroy()
    {
        if(!mSDLTex)
            return;

        SDL_DestroyTexture(mSDLTex);
        mSDLTex = nullptr;
        mName = "";
    }
    
    bool Texture::TextureInitialised() const
    {

    }

    void Texture::SetTextureName(const std::string& newName)
    {
        mName = newName;
    }

    bool Texture::LoadTexture(const std::string& filePath)
    {
        if(TextureInitialised())
            Destroy();

        mSDLTex = IMG_LoadTexture(mWindow.GetRenderer(), filePath.c_str());

        if(mSDLTex == NULL)
        {
            LogMsg(ERROR, "failed to load texture at file path '%s'. IMG_ERROR: ", IMG_GetError());
            return false;
        }


    }
};
