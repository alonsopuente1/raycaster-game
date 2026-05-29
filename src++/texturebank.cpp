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

    void TextureBank::RemoveByName(const std::string &pName)
    {
        for(int i = 0; i < mTextures.size(); i++)
        {
            if(mTextures[i].GetTextureName() == pName)
            {
                mTextures.erase(mTextures.begin() + i);
                return;
            }
        }
    }

    inline size_t TextureBank::BankSize() const
    {
        return mTextures.size();
    }


};