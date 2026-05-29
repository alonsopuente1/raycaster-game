#include "window.hpp"
#include "texture.hpp"
#include "map.hpp"

#include "logger.hpp"

int main(int argc, char** argv)
{
    CastEngine::Window myWnd;

    if(!myWnd.CreateWindow("RayCaster", 1280, 720))
    {
        LogMsg(ERROR, "failed to create window");
        return -1;
    }

    bool running = true;

    CastEngine::Texture myTex(myWnd);

    if(!myTex.LoadTexture("res/textures/enemies/cacodemon.png"))
    {
        LogMsg(ERROR, "failed to load texture"); 
        return -1;
    }

    CastEngine::Map myMap;

    CastEngine::Map::LoadArgs args;
    myMap.LoadMap("res/maps/map1.sdm", args);

    myMap.print();

    while(running)
    {
        SDL_Event e;

        while(SDL_PollEvent(&e))
        {
            switch(e.type)
            {
                case SDL_QUIT:
                    running = false;
                    break;
            }
        }

        SDL_Rect dst = { 0, 0, myWnd.GetWidth() / 2, myWnd.GetHeight() / 2};
        
        if(SDL_RenderCopy(myWnd.GetRenderer(), myTex.GetTexture(), NULL, &dst) < 0)
        {
            LogMsgf(ERROR, "failed to render texture. SDL_ERROR: %s", SDL_GetError());
        }

        SDL_RenderPresent(myWnd.GetRenderer());
    }

    return 0;
}