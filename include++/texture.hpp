#pragma once

#include <SDL2/SDL.h>
#include <stdint.h>
#include <string>

/* texture.hpp



*/

namespace CastEngine
{

    // forward decleration
    class Window;

    /// @brief texture class for handling textures in the engine
    /// MUST be created with a window
    class Texture 
    {

    private:
        SDL_Texture*    mSDLTex;
        // name of texture. can be used as unique id or not.
        // usually holds the file path from the last image it loaded
        std::string     mName;
    
        Window&         mWindow;

    public:

        /// @brief textures have to be attached to a window
        ///     the textures only work for that window
        /// @param window window to attach to
        Texture(Window& window);

        ~Texture();

        /// @brief textures have to be attached to a window
        ///     the textures only work for that window
        /// @param window window to attach to
        /// @param file file path to the image to load
        Texture(Window& window, const std::string& file);
        
        /// @brief copies the texture including its name
        /// @param other other texture to copy
        Texture(const Texture& other);

        // delete copy assignment
        Texture& operator=(const Texture&);

        // move constructors
        Texture(Texture&& other);
        Texture& operator=(Texture&& tex);
        
        /// @brief textures have to be attached to a window.
        /// @brief the textures only work for the window attached  
        /// @param window window to attach to
        /// @param name name to give to texture
        /// @param width width of the texture 
        Texture(Window& window, const std::string& name, int width, int height);

        /// @brief getter method for width
        /// @return width of texture (0 if no texture loaded)
        uint32_t GetWidth() const;

        /// @brief getter method for height
        /// @return height of texture (0 if no texture loaded)
        uint32_t GetHeight() const;

        /// @brief sets the name of texture
        /// @param newName name to set to the texture
        /// @return 
        void SetTextureName(const std::string& newName);

        const std::string& GetTextureName() const;

        /// @brief destroys the texture but keeps window attached
        void Destroy();
        
        /// @brief returns if texture exists or not
        /// @return true if does exist, false if not
        bool IsInitialised() const;

        /// @brief loads a texture from image file. replaces any existing texture
        /// @param filePath path to image file
        bool LoadTexture(const std::string& filePath);
                
        /// @brief getter method for internal SDL texture
        /// @return NULL if not initialised
        SDL_Texture* GetTexture() const;

        /// @brief destroys current texture and replaces with blank texture
        /// @param pWidth width of new texture
        /// @param pHeight height of new texture;
        /// @return true if successful, false if not
        bool CreateBlankTexture(const std::string& pName, int pWidth, int pHeight);

        Window& GetAttachedWindow() const;
        void SetAttachedWindow(Window& pWindow);

    };
};