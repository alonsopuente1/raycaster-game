#pragma once

#include "texture.hpp"

#include <vector>

namespace CastEngine
{
    class TextureBank
    {

    public:

        const Texture& operator[] (int i);
        const Texture* operator[] (const std::string& str);
        
        void FreeAll();
        void RemoveByName(const std::string& pName);

        size_t NumTextures() const;


    private:

        std::vector<Texture> mTextures;

    };
};