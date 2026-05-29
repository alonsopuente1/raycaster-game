#pragma once

#include "texturebank.hpp"

namespace CastEngine
{
  
    class Window;
    class Player;
    class Map;
    class Gun;
    class EntityManager;

    /// @brief a renderer for rendering player views, entities and more
    /// should be created alongside a Window object
    class Renderer
    {

    private:
        
        Window& mWindow;

        std::vector<float> depthBuffer;

        void ResetDepthBuffer();
        
    public:
        
        Renderer(Window& window);
        ~Renderer();
        
        TextureBank texBank;

        inline Window& GetWindow() { return mWindow; }

        bool RenderTexture(const Texture& tex, SDL_Rect src, SDL_Rect dst);

        void RenderPlayerView(const Player& pPlayer, const Map& pMap);
        void RenderGun(const Gun& pGun);
        void RenderCeilingAndFloor(SDL_Colour topColour, SDL_Colour bottomColour);
        
        void RenderMinimap(const Player& pPlayer, const EntityManager& pEm, const Map& map);

        void UpdateMinimap(const Player& pPlayer, const EntityManager& pEm, const Map& pMap);
        
        void ClearScreen(SDL_Color& colour);
        void Present();

        void Destroy();

        Window& GetWindow() const;

    };

};