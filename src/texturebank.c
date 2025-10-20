#include "texturebank.h"

#include "logger.h"
#include "window.h"

// returns null when theres no textures allocated
struct texturenode_s* TB_GetLastNode(texturebank_t* texturebank)
{
    struct texturenode_s* headNode = texturebank->headNode;
    if(!headNode)
        return headNode;

    while(headNode->nextNode)
    {
        headNode = headNode->nextNode;
    }

    return headNode;
}

texture_t* TB_AddEmptyTexture(texturebank_t* texturebank)
{
    struct texturenode_s* tail = TB_GetLastNode(texturebank);

    struct texturenode_s** destination = NULL;

    if(!tail)
        destination = &texturebank->headNode;
    else
        destination = &tail->nextNode;

    *destination = calloc(1, sizeof(struct texturenode_s));
    if(!destination)
    {
        LogMsg(ERROR, "failed to allocate memory for empty texture\n");
        return NULL;
    }

    return &(*destination)->data;
}

bool TB_PushTexture(window_t* window, texturebank_t* texturebank, const char* filepath)
{
    if(!texturebank)
    {
        LogMsg(WARN, "passed null to texture bank\n");
        return false;
    }

    struct texturenode_s* tail = TB_GetLastNode(texturebank);

    // when theres no textures, allocate memory for one
    if(!tail)
    {
        texturebank->headNode = calloc(1, sizeof(struct texturenode_s));
        if(!texturebank->headNode)
        {
            LogMsgf(ERROR, "failed to allocate memory for texture at filepath '%s'\n" , filepath);
            return false;
        }

        tail = texturebank->headNode;
    }
    else
    {
        tail->nextNode = calloc(1, sizeof(struct texturenode_s));
        if(!tail->nextNode)
        {
            LogMsgf(ERROR, "failed to allocate memory for texture at filepath '%s'\n" , filepath);
            return false;
        }

        tail = tail->nextNode;
    }

    tail->data = T_LoadTexture(window, filepath);
    if(!tail->data.data)
    {
        LogMsgf(ERROR, "failed to push texture at file path '%s'\n", filepath);
        return false;
    }

    tail->nextNode = NULL;

    return true;
}

int TB_NumTextures(texturebank_t* texturebank)
{
    int i = 0;

    struct texturenode_s* currentNode = texturebank->headNode;
    for(; currentNode; i++, currentNode = currentNode->nextNode);

    return i;
}

void TB_FreeAllTextures(texturebank_t* texturebank)
{
    if(!texturebank)
    {
        LogMsg(WARN, "passed null to texture bank\n");
        return;
    }

    struct texturenode_s* currentNode = texturebank->headNode;
    
    while(currentNode)
    {
        struct texturenode_s* temp = currentNode;
        currentNode = currentNode->nextNode;
        T_FreeTexture(&temp->data);
        free(temp);
    }
}

// returns true on success, false on failure
bool TB_RemoveTexture(texturebank_t* texturebank, const char* texturename)
{
    if(!texturebank)
    {
        LogMsg(WARN, "passed null to texture bank\n");
        return false;
    }

    struct texturenode_s* prevNode = NULL;
    struct texturenode_s* currentNode = texturebank->headNode;

    while(currentNode)
    {
        if(strcmp(currentNode->data.name, texturename) == 0)
        {
            // this means that the head of the linked list is the texture being searched for
            if(!prevNode)
                texturebank->headNode = texturebank->headNode->nextNode;

            prevNode->nextNode = currentNode->nextNode;

            T_FreeTexture(&currentNode->data);
            free(currentNode);
            return true;
        }

        prevNode = currentNode;
        currentNode = currentNode->nextNode;
    }

    return false;
}

/// @brief returns the texture in the index position in the texture bank
/// @param texturebank texturebank to find texture from
/// @param index index of textures in order
/// @return NULL on error, either when texture bank is null, index out of range 
texture_t* TB_GetTextureByIndex(texturebank_t* texturebank, int index)
{
    if(!texturebank)
    {
        LogMsg(WARN, "passing null for texture bank\n");
        return NULL;
    }

    struct texturenode_s* currentNode = texturebank->headNode;

    for(int i = 0; i < index && currentNode; i++, currentNode = currentNode->nextNode);

    return &currentNode->data;
}

texture_t* TB_FindTexture(texturebank_t* texturebank, const char* texturename)
{
    if(!texturebank)
    {
        LogMsg(WARN, "passed null to texture bank\n");
        return NULL;
    }

    struct texturenode_s* currentNode = texturebank->headNode;

    while(currentNode)
    {
        if(strcmp(currentNode->data.name, texturename) == 0)
        {
            return &currentNode->data;
        }

        currentNode = currentNode->nextNode;
    }

    return NULL;
}