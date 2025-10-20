#ifndef __TEXTUREBANK_H__
#define __TEXTUREBANK_H__

#include <stdbool.h>

#include "texture.h"
#include "window.h"

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
extern bool TB_RemoveTexture(texturebank_t* texturebank, const char* texturename);
// adds a texture to the texturebank without any data
// returns null on failure, pointer to empty texture on success
extern texture_t* TB_AddEmptyTexture(texturebank_t* texturebank);

extern texture_t* TB_GetTextureByIndex(texturebank_t* texturebank, int index);

extern int TB_NumTextures(texturebank_t* texturebank);

// returns NULL if couldnt find texture
extern texture_t* TB_FindTexture(texturebank_t* texturebank, const char* texturename);

#endif