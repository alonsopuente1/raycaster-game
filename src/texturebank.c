#include "texturebank.h"

#include "logger.h"
#include "w_window.h"
#include "util.h"

/* FORWARD DECLERATIONS */

extern struct texturenode_s* TB_GetLastNode(texturebank_t* texturebank);
extern void TB_DeleteLastNode(texturebank_t* texturebank);
extern texture_t* TB_AddEmptyTexture(texturebank_t* texturebank);

/* PUBLIC FUNCTIONS */

texture_t* TB_AddAndCreateEmptyTexture(texturebank_t* texturebank, window_t* window, const char* name, int width, int height)
{
    if(!texturebank)
    {
        LogMsg(ERROR, "passed null ptr to texturebank");
        return NULL;
    }

    if(!name || strlen(name) <= 0)
    {
        LogMsg(ERROR, "name is null or empty");
        return NULL;
    }

    if(width <= 0 || height <= 0)
    {
        LogMsgf(ERROR, "passed invalid width or height: %ix%i", width, height);
        return NULL;
    }

    if(TB_FindTextureByName(texturebank, name))
    {
        LogMsgf(ERROR, "texture with name '%s' already exists in texturebank", name);
        return NULL;
    }

    // cant return null since the program crashes on fail in this function
    texture_t* texture = TB_AddEmptyTexture(texturebank);

    *texture = T_CreateBlankTexture(window, name, width, height);
    if(!texture->data)
    {
        LogMsgf(ERROR, "failed to create blank texture '%s' when adding empty texture to texturebank", name);
        TB_DeleteLastTexture(texturebank);
        return NULL;
    }

    return texture;
}

bool TB_PushTexture(window_t* window, texturebank_t* texturebank, const char* filepath)
{
    if(!texturebank)
    {
        LogMsg(WARN, "passed null to texture bank\n");
        return false;
    }

    char fileName[512] = { 0 };
    fileNameFromPath(filepath, fileName, sizeof(fileName));

    // check if file has already been loaded
    if(TB_FindTextureByName(texturebank, fileName))
    {
        LogMsgf(WARN, "texture with name '%s' has already been loaded into the texturebank", fileName);
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
            exit(-1);
        }

        tail = texturebank->headNode;
    }
    else
    {
        tail->nextNode = calloc(1, sizeof(struct texturenode_s));
        if(!tail->nextNode)
        {
            LogMsgf(ERROR, "failed to allocate memory for texture at filepath '%s'\n" , filepath);
            exit(-1);
        }

        tail = tail->nextNode;
    }

    tail->data = T_LoadTexture(window, filepath);
    if(!tail->data.data) // TODO: fix potential memory leak
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

// safe to pass null
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

    texturebank->headNode = NULL;
}

// returns true on success, false on failure
bool TB_RemoveTextureByName(texturebank_t* texturebank, const char* texturename)
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

bool TB_RemoveTextureByPtr(texturebank_t* texturebank, texture_t* ptr)
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
        if(&currentNode->data == ptr)
        {
            // this means that the head of the linked list is the texture being searched for
            if(!prevNode)
                texturebank->headNode = texturebank->headNode->nextNode;
            else
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

texture_t* TB_FindTextureByName(texturebank_t* texturebank, const char* texturename)
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

bool TB_IsTexInTextureBank(texturebank_t* texturebank, texture_t* ptr)
{
    if(!texturebank)
    {
        LogMsg(WARN, "passed null to texture bank\n");
        return false;
    }

    struct texturenode_s* currentNode = texturebank->headNode;

    while(currentNode)
    {
        if(&currentNode->data == ptr)
        {
            return true;
        }

        currentNode = currentNode->nextNode;
    }

    return false;
}

// deletes the last node of the texturebank
void TB_DeleteLastTexture(texturebank_t* texturebank)
{
    if(!texturebank)
        return;
    if(!texturebank->headNode)
        return;

    // this means that there is only 1 node in list which is head node
    if(!texturebank->headNode->nextNode)
    {
        T_FreeTexture(&texturebank->headNode->data);
        free(texturebank->headNode);
        texturebank->headNode = NULL;
        return;
    }

    // there is at least 2 textures by this point so while loop is safe
    struct texturenode_s* currentNode = texturebank->headNode;

    while(currentNode->nextNode->nextNode)
        currentNode = currentNode->nextNode;

    T_FreeTexture(&currentNode->nextNode);
    free(currentNode->nextNode);
    currentNode->nextNode = NULL;
}

/* PRIVATE FUNCTIONS */

// returns NULL when cant allocate memory, 
texture_t* TB_AddEmptyTexture(texturebank_t* texturebank)
{
    struct texturenode_s* tail = TB_GetLastNode(texturebank);

    struct texturenode_s** destination = NULL;

    if(!tail)
        destination = &texturebank->headNode;
    else
        destination = &tail->nextNode;

    *destination = calloc(1, sizeof(struct texturenode_s));
    if(!(*destination))
    {
        LogMsg(ERROR, "failed to allocate memory for empty texture\n");
        exit(-1);
    }

    return &(*destination)->data;
}

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
