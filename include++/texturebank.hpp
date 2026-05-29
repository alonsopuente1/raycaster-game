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

        /// @brief size of the internal vector of Texture's
        /// @return size of vector
        inline size_t BankSize() const;

        void PushTexture(Texture&& tex);

    private:

        std::vector<Texture> mTextures;

    };
};