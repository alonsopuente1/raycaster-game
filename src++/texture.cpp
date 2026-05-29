#include "texture.hpp"

#include <SDL2/SDL_image.h>

#include "logger.hpp"

#include "window.hpp"

namespace CastEngine
{
 
    Texture::Texture(Window& window) : mWindow(window), mSDLTex(NULL), mName("")
    {}
    
    Texture::Texture(Window& window, const std::string& file) : mWindow(window), mSDLTex(NULL), mName("")
    {
        LoadTexture(file);
    }

    Texture::Texture(Texture &&other) : mWindow(other.mWindow), mSDLTex(other.mSDLTex), mName(other.mName)
    {
        other.mSDLTex = NULL;
        other.mName = "";
    }

    Texture &Texture::operator=(Texture &&tex)
    {
        if(this == &tex)
            return *this;

        if(mSDLTex)
            SDL_DestroyTexture(mSDLTex);

        mSDLTex = tex.mSDLTex;

        tex.mSDLTex = NULL;

        return *this;
    }

    Texture::Texture(Window& window, const std::string& name, int width, int height) : mWindow(window), mSDLTex(NULL), mName("")
    {
        CreateBlankTexture(name, width, height);
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
            LogMsgf(ERROR, "failed to retrieve width of texture (%s)", mName.c_str());
            return 0;
        }
        return width;
    }
    uint32_t Texture::GetHeight() const
    {
        int height;
        if(SDL_QueryTexture(mSDLTex, NULL, NULL, NULL, &height) < 0)
        {
            LogMsgf(ERROR, "failed to retrieve height of texture (%s)", mName.c_str());
            return 0;
        }
        return height;
    }
    
    void Texture::Destroy()
    {
        if(mSDLTex)
            SDL_DestroyTexture(mSDLTex);
        mSDLTex = nullptr;
        mName = "";
    }
    
    bool Texture::IsInitialised() const
    {
        return mSDLTex != NULL;
    }

    void Texture::SetTextureName(const std::string& newName)
    {
        mName = newName;
    }

    const std::string &Texture::GetTextureName() const
    {
        return mName;
    }

    bool Texture::LoadTexture(const std::string& filePath)
    {
        Destroy();

        mSDLTex = IMG_LoadTexture(mWindow.GetRenderer(), filePath.c_str());

        if(mSDLTex == NULL)
        {
            LogMsgf(ERROR, "failed to load texture at file path '%s'. IMG_ERROR: ", IMG_GetError());
            return false;
        }

        return true;
    }
    SDL_Texture *Texture::GetTexture() const
    {
        return mSDLTex;
    }
    bool Texture::CreateBlankTexture(const std::string &pName, int pWidth, int pHeight)
    {
        if(!mWindow.IsInitialised())
        {
            LogMsg(ERROR, "attempting to create texture on uninitialised window");
            return false;
        }
        
        // create new blank texture
        SDL_Texture* newTex = SDL_CreateTexture(mWindow.GetRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, pWidth, pHeight);
        if(newTex == NULL)
        {
            LogMsgf(ERROR, "failed to create blank texture. SDL_ERROR: %s", SDL_GetError());
            return false;
        }
        
        // only destroy after checking texture was able to be created
        if(IsInitialised())
            Destroy();

        mSDLTex = newTex;        

        return true;
    }
    Window &Texture::GetAttachedWindow() const
    {
        return mWindow;
    }

    void Texture::SetAttachedWindow(Window& pWindow)
    {
        if(IsInitialised())
            LogMsgf(WARN, "setting new attached window without destroying old texture (%s)... destroying texture...", mName.c_str());

        Destroy();

        mWindow = pWindow;
    }


};
