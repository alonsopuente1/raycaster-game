#ifndef __TEXTUREBANK_H__
#define __TEXTUREBANK_H__

#include <stdbool.h>

#include "texture.h"
#include "w_window.h"

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
extern bool TB_RemoveTextureByName(texturebank_t* texturebank, const char* texturename);\
extern bool TB_RemoveTextureByPtr(texturebank_t* texturebank, texture_t* ptr);\
// adds a texture to the texturebank with texture_t->data == NULL
// returns null on failure, pointer to empty texture on success
extern texture_t* TB_AddEmptyTexture(texturebank_t* texturebank);

extern texture_t* TB_GetTextureByIndex(texturebank_t* texturebank, int index);

extern int TB_NumTextures(texturebank_t* texturebank);

// returns NULL if couldnt find texture
extern texture_t* TB_FindTextureByName(texturebank_t* texturebank, const char* texturename);
extern bool TB_IsTexInTextureBank(texturebank_t* texturebank, texture_t* ptr);

#endif