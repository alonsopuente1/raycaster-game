#include "s_scene.h"

#include "m_game.h"

#include "logger.h"

scene_t S_CreateScene(maingame_t* parentGame, void* scene, unsigned int size, const char* sceneName)
{
    // checks
    if(!parentGame)
    {
        LogMsg(WARN, "passed null ptr to parentGame\n");
        return (scene_t){0};
    }

    if(!scene)
    {
        LogMsg(WARN, "passed null ptr to scene\n");
        return (scene_t){0};
    }

    scene_t newScene = { 0 };

    int paramSceneNameLen = strlen(sceneName);
    if(paramSceneNameLen >= 32)
        LogMsgf(WARN, "scene name '%s' will be truncated\n", sceneName);

    strncpy(newScene.sceneName, sceneName, paramSceneNameLen);
    newScene.sceneName[31] = '\0';

    newScene.parentGame = parentGame;

    newScene.data = malloc(size);
    if(!newScene.data)
    {
        LogMsg(ERROR, "failed to allocate memory for scene\n");
        return (scene_t){0};
    }

    memcpy(newScene.data, scene, size);
    memset(newScene.data, 0, size);

    printf("size: %i\n", size);

    SetupFunc* setupPtr = (SetupFunc*)scene;
    HandleEventsFunc* handleEventsPtr = (HandleEventsFunc*)((char*)setupPtr + sizeof(SetupFunc));
    UpdateFunc* updatePtr = (UpdateFunc*)((char*)handleEventsPtr + sizeof(HandleEventsFunc));
    DrawFunc* drawPtr = (DrawFunc*)((char*)updatePtr + sizeof(UpdateFunc));
    DestroyFunc* destroyPtr = (DestroyFunc*)((char*)drawPtr + sizeof(DrawFunc));

    newScene.setup = *setupPtr;
    if(!newScene.setup)
    {
        LogMsg(WARN, "scene created with null setup function pointer. did you order the scene's members correctly?\n");
        free(newScene.data);
        return (scene_t){0}; 
    }

    newScene.handleEvents = *handleEventsPtr;
    if(!newScene.handleEvents)
    {
        LogMsg(WARN, "scene created with null handleEvents function pointer. did you order the scene's members correctly?\n");
        free(newScene.data);
        return (scene_t){0}; 
    }

    newScene.update = *updatePtr;
    if(!newScene.update)
    {
        LogMsg(WARN, "scene created with null update function pointer. did you order the scene's members correctly?\n");
        free(newScene.data);
        return (scene_t){0}; 
    }

    newScene.draw = *drawPtr;
    if(!newScene.draw)
    {
        LogMsg(WARN, "scene created with null draw function pointer. did you order the scene's members correctly?\n");
        free(newScene.data);
        return (scene_t){0};
    }

    newScene.destroy = *destroyPtr;
    if(!newScene.destroy)
    {
        LogMsg(WARN, "scene created with null destroy function pointer. did you order the scene's members correctly?\n");
        free(newScene.data);
        return (scene_t){0};
    }

    return newScene;
}

void S_CallSetup(scene_t* scene)
{
    if(!scene)
    {
        LogMsg(WARN, "passed null ptr to scene\n");
        return;
    }

    if(!scene->parentGame)
    {
        LogMsg(WARN, "scene has nullptr for game. something is very wrong\n");
        return;
    }

    if(!scene->data)
    {
        LogMsg(WARN, "scene data is null\n");
        return;
    }

    scene->setup(scene->data, scene->parentGame);
}

void S_CallHandleEvents(scene_t* scene, SDL_Event* event)
{
    if(!scene)
    {
        LogMsg(WARN, "passed null ptr to scene\n");
        return;
    }

    if(!scene->parentGame)
    {
        LogMsg(WARN, "scene has nullptr for game. something is very wrong\n");
        return;
    }

    if(!event)
    {
        LogMsg(WARN, "passed null ptr to event\n");
        return;
    }

    if(!scene->data)
    {
        LogMsg(WARN, "scene data is null\n");
        return;
    }

    scene->handleEvents(scene->data, scene->parentGame, event);
}

void S_CallUpdate(scene_t* scene, float dt)
{
    if(!scene)
    {
        LogMsg(WARN, "passed null ptr to scene\n");
        return;
    }

    if(!scene->parentGame)
    {
        LogMsg(WARN, "scene has nullptr for game. something is very wrong\n");
        return;
    }

    if(!scene->data)
    {
        LogMsg(WARN, "scene data is null\n");
        return;
    }

    scene->update(scene->data, scene->parentGame, dt);
}

void S_CallDraw(scene_t* scene)
{
    if(!scene)
    {
        LogMsg(WARN, "passed null ptr to scene\n");
        return;
    }

    if(!scene->parentGame)
    {
        LogMsg(WARN, "scene has nullptr for game. something is very wrong\n");
        return;
    }

    if(!scene->data)
    {
        LogMsg(WARN, "scene data is null\n");
        return;
    }

    scene->draw(scene->data, scene->parentGame);
}

void S_CallDestroy(scene_t* scene)
{
    if(!scene)
    {
        LogMsg(WARN, "passed null ptr to scene\n");
        return;
    }

    if(!scene->parentGame)
    {
        LogMsg(WARN, "scene has nullptr for game. something is very wrong\n");
        return;
    }

    if(!scene->data)
    {
        LogMsg(WARN, "scene data is null\n");
        return;
    }

    scene->destroy(scene->data, scene->parentGame);
}

void S_DestroyScene(scene_t* scene)
{
    if(scene->data)
    {
        free(scene->data);
        scene->data = NULL;
    }

    scene->handleEvents = NULL;
    scene->update = NULL;
    scene->draw = NULL;
    scene->parentGame = NULL;
    scene->sceneName[0] = '\0';
}
