#pragma once

#include "texturebank.hpp"

namespace CastEngine
{
  
    class Window;
    class Player;
    class Map;
    class Gun;
    class EntityManager;

    class Renderer
    {

    private:
        
        Window& mWindow;

        std::vector<float> depthBuffer;

        TextureBank mTexBank;

    public:

        Renderer(Window& window);
        ~Renderer();

        void RenderPlayerView(const Player& pPlayer, const Map& pMap);
        void RenderGun(const Gun& pGun);
        void RenderCeilingAndFloor(SDL_Colour topColour, SDL_Colour bottomColour);
        
        void RenderMinimap(const Player& pPlayer, const EntityManager& pEm, const Map& map);

        void UpdateMinimap(const Player& pPlayer, const EntityManager& pEm, const Map& pMap);
        
        void ClearScreen(SDL_Color& colour);
        void Present();

        void Destroy();

    };

};