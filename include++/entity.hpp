#pragma once

namespace CastEngine
{
    class Texture;
    class Map;
    class Entity
    {

    private:

        Texture* mTexture;

        Map& mMap;
        

    public:

        Entity(Map& pMap) : mTexture(nullptr), mMap(pMap) {}
        ~Entity() {}

        void SetTexture(const Texture& tex);
        const Texture* GetTexture();



    };

};