#ifndef __TEXTUREBANK_H__
#define __TEXTUREBANK_H__

#include <stdbool.h>

#include "texture.h"
#include "w_window.h"

/* texturebank.h

    defines a texturebank which is a glorified linked list

    simply just a linked list which stores only textures
    contains helper functions   
*/

struct texturenode_s 
{
    texture_t               data;
    struct texturenode_s*    nextNode;
};

typedef struct texturebank_s
{
    struct texturenode_s*    headNode;
} texturebank_t;


// returns true on success, false on failure
extern bool TB_PushTexture(window_t* window, texturebank_t* texturebank, const char* filepath);
extern void TB_FreeAllTextures(texturebank_t* texturebank);
extern bool TB_RemoveTextureByName(texturebank_t* texturebank, const char* texturename);
extern bool TB_RemoveTextureByPtr(texturebank_t* texturebank, texture_t* ptr);

// frees last node of the texturebank's linked list
extern void TB_DeleteLastTexture(texturebank_t* texturebank);
// adds a texture to the texturebank and returns a pointer to it
// the returned texture will have the data already allocated
extern texture_t* TB_AddAndCreateEmptyTexture(texturebank_t* texturebank, window_t* window, const char* name, int width, int height);

// traverses the linked list by index times
extern texture_t* TB_GetTextureByIndex(texturebank_t* texturebank, int index);

extern int TB_NumTextures(texturebank_t* texturebank);

// returns first iteration of the texture by name
//   this means that there can be more than one texture with the same name
// returns NULL if couldnt find texture
extern texture_t* TB_FindTextureByName(texturebank_t* texturebank, const char* texturename);
extern bool TB_IsTexInTextureBank(texturebank_t* texturebank, texture_t* ptr);

#endif