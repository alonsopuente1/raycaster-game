#pragma once

#include <SDL2/SDL.h>
#include <stdint.h>
#include <string>

namespace CastEngine
{

    class Window
    {

    private:

        SDL_Window* mSDLWindow;
        SDL_Renderer* mSDLRenderer;

    public:
    
        /// @brief constructor, defaults all internals to NULL
        Window();

        /// @brief constructor for creating a window straight away
        /// @param title title of window to be created
        /// @param pWidth width of window
        /// @param pHeight height of window
        Window(const std::string& title, int pWidth, int pHeight);

        /// @brief destructor of window
        ~Window();

        /// @brief destroys internal SDL_Window + SDL_Renderer 
        /// and sets them to NULL
        void Destroy();

        /// @brief creates a new window. if a window and 
        /// renderer already exist, it gets destroyed, effectively
        /// making all textures creating on the renderer useless
        /// @param title title of the window to be created
        /// @param pWidth width of the window to be created
        /// @param pHeight height of the window to be created
        /// @return true if successful, false if not. prints reason why if not
        bool CreateWindow(const std::string& title, int pWidth, int pHeight);

        /// @brief sets title of the window
        /// @param newTitle new title of window
        void SetTitle(const std::string& newTitle);

        /// @brief gets the current title of the window
        /// @return title of window
        const char* GetTitle() const;

        /// @brief gets the current width of the window
        /// @return width of window
        int GetWidth() const;
        
        /// @brief gets the current height of the window
        /// @return height of window
        int GetHeight() const;

        /// @brief gets the internal SDL_Renderer*
        /// @return NULL if uninitialised
        SDL_Renderer* GetRenderer() const;

        /// @brief gets the internal SDL_Window*
        /// @return NULL if uninitialised
        SDL_Window* GetWindow() const;

        /// @brief checks if the window is already initialised or not
        /// @return true if initialised, false if not
        bool IsInitialised() const;

        bool operator==(Window& other) const;
        bool operator!=(Window& other) const;
    };

};