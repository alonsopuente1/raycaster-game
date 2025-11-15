#ifndef __S_SCENE_H__
#define __S_SCENE_H__

#include <SDL2/SDL.h>

/* s_scene.h

    this is how user creates scenes for the engine

    scene system for managing different scenes (menus, gameplay, etc.) in the game.
    for user defined scenes, create a struct that MUST HAVE THE FOLLOWING FUNCTION POINTERS
    AS ITS FIRST MEMBERS:
        SetupFunc           setup;
        HandleEventsFunc    handleEvents;
        UpdateFunc          update;
        DrawFunc            draw;
        DestroyFunc         destroy;

    then create functions that match the signatures of those function pointers.

    function signatures:
        -SetupFunc:         void Setup(void* scene, maingame_t* game)
        -HandleEventsFunc:  void HandleEvents(void* scene, maingame_t* game, SDL_Event* event)
        -UpdateFunc:        void Update(void* scene, maingame_t* game, float dt)
        -DrawFunc:          void Draw(void* scene, maingame_t* game)
        -DestroyFunc:       void Destroy(void* scene, maingame_t* game)

    function descriptions
        -SetupFunc:         called when scene is loaded. this when the scene is switched to.
        -HandleEventsFunc:  called every frame to handle input/events.
        -UpdateFunc:        called every frame to update scene logic.
        -DrawFunc:          called every frame to render the scene.
        -DestroyFunc:       called when scene is unloaded. this is when the scene is switched away from.

    when creating a scene, pass a pointer to an instance of that struct to S_CreateScene(),
    along with the parent maingame_t pointer and a name for the scene.
    this way, scenes can switch to other scenes using G_ChangeScene()

                                                        | pointer to user defined scene struct
    to add a scene to the game, use G_AddScene(void* scene, const char* sceneName)
                                    or G_AddSceneExisting(scene_t scene)
                                    

    TODO:   use zone system for scene data management so that all scene data is allocated/freed
            when the scene is created/destroyed and user doesn't have to creat new structs for each scene
            and manually manage memory for them.

*/

typedef struct maingame_s maingame_t;

// Function pointer typedefs for scene methods

// first parameter is a pointer to the user-defined scene struct
typedef void (*SetupFunc)(void*, maingame_t*);
// first parameter is a pointer to the user-defined scene struct
typedef void (*HandleEventsFunc)(void*, maingame_t*, SDL_Event*);
// first parameter is a pointer to the user-defined scene struct
typedef void (*UpdateFunc)(void*, maingame_t*, float);
// first parameter is a pointer to the user-defined scene struct
typedef void (*DrawFunc)(void*, maingame_t*);
// first parameter is a pointer to the user-defined scene struct
typedef void (*DestroyFunc)(void*, maingame_t*);

typedef struct scene_s
{
    void*               data;

    char                sceneName[32];

    SetupFunc           setup;
    HandleEventsFunc    handleEvents;
    UpdateFunc          update;
    DrawFunc            draw;
    DestroyFunc         destroy;

    maingame_t*         parentGame;
} scene_t;

extern scene_t S_CreateScene(maingame_t* parentGame, void* scene, unsigned int size, const char* sceneName);

extern void S_CallSetup(scene_t* scene);
extern void S_CallHandleEvents(scene_t* scene, SDL_Event* event);
extern void S_CallUpdate(scene_t* scene, float dt);
extern void S_CallDraw(scene_t* scene);
extern void S_CallDestroy(scene_t* scene);

extern void S_DestroyScene(scene_t* scene);

#endif