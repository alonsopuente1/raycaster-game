#ifndef __M_GAME_H__
#define __M_GAME_H__

#include "map.h"
#include "w_window.h"
#include "texturebank.h"
#include "p_player.h"

#include "s_scene.h"

#include <stdbool.h>
#include <SDL2/SDL_mixer.h>

/* m_game.h

    defines the maingame struct which manages user-defined scenes, 
    the main application window and a texture bank
    
    the maingame struct simply calls the user-defined scene's HandleEvents, 
    Update and Draw functions, so this struct is simply used as a manager 
    for the scenes

*/

typedef struct winButton_s winButton_t;
typedef struct player_s player_t;

typedef struct maingame_s
{
    window_t        window;

    bool            running;

    scene_t*        currentScene;
    scene_t*        scenes;
    int             numScenes;
    
} maingame_t;


extern bool G_Init(maingame_t* game);

/// @brief adds a user-defined scene to the game for use
/// @param game pointer to instance of maingame_t to add the scene to
/// @param scene pointer to instance of the user-defined scene. doesn't matter if it's heap or stack allocated since the data inside the struct gets copied to the scene inside the game
/// @param size size of the user-defined struct in bytes. just use sizeof()
/// @param sceneName name of the scene so that you can change to the scene using G_ChangeScene()
/// @return true if successful, false if not
extern bool G_AddScene(maingame_t* game, void* scene, unsigned int size, const char* sceneName);

// scene should be created beforehand using S_CreateScene()
extern void G_AddSceneExisting(maingame_t* game, scene_t* scene);
// destroys the scene currently loaded and creates the new scene
// by name. 
// IF THIS FUNCTION IS USED INSIDE OF ANYWHERE OF ANOTHER SCENE'S LOGIC,
// USING ANY DATA MEMBERS BELONGING TO THAT SCENE WILL RESULT IN A
// SEG FAULT
extern void G_ChangeScene(maingame_t* game, const char* sceneName);
extern void G_Run(maingame_t* game);
extern void G_Destroy(maingame_t* game);

#endif