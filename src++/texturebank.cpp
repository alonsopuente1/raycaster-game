#include "texturebank.hpp"

namespace CastEngine
{

    const Texture& TextureBank::operator[] (int i )
    {
        return mTextures[i];
    }

    const Texture* TextureBank::operator[](const std::string &str)
    {
        for(Texture& tex : mTextures)
            if (tex.GetTextureName() == str)
                return &tex;

        return NULL;
    }
    size_t TextureBank::NumTextures() const
    {
        return mTextures.size();
    }
};