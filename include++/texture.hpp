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

        /// @brief textures have to be attached to a window
        ///     the textures only work for that window
        /// @param window window to attach to
        /// @param file 
        Texture(Window& window, const std::string& file);
        
        
        /// @brief textures have to be attached to a window
        ///     the textures only work for that window
        ///     creates a blank texture of the dimensions passed and 
        /// @param window window to attach to
        /// @param name name to give to texture
        /// @param width width of the texture 
        Texture(Window& window, const std::string& name, int width, int height);

        /// @brief essentially calls Texture::Destroy()
        ~Texture();

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

        /// @brief destroys and resets all member variables of Texture
        void Destroy();
        
        /// @brief returns if texture is initialised or not
        /// @return true if initialised false if not
        bool TextureInitialised() const;

        /// @brief loads a texture from image file. replaces any existing texture
        /// @param filePath path to image file
        bool LoadTexture(const std::string& filePath);
    };
};