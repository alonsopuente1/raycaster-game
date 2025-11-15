#include "m_game.h"

#include "s_mainmenuscene.h"
#include "s_gamescene.h"

#include "logger.h"

#ifdef linux
#error linux support not implemented yet
#endif

maingame_t game = { 0 };

int main(int argc, char** argv)
{
    LogMsg(DEBUG, "Starting RayCaster...\n");

    mainMenuScene_t mmScene = {
        .setup = MMS_SetupScene,
        .handleEvents = MMS_HandleEvents,
        .update = MMS_Update,
        .draw = MMS_Draw,
        .destroy = MMS_Destroy
    };

    gamescene_t gScene = {
        .setup = GS_SetupScene,
        .handleEvents = GS_HandleEvents,
        .update = GS_Update,
        .draw = GS_Draw,
        .destroy = GS_DestroyScene
    };

    if(!G_Init(&game))
        return -1;

    if(!G_AddScene(&game, &mmScene, sizeof(mmScene), "MainMenu"))
        return -1;

    if(!G_AddScene(&game, &gScene, sizeof(gScene), "Game"))
        return -1;
        
    G_ChangeScene(&game, "MainMenu");

    G_Run(&game);

    G_Destroy(&game);

    return 0;
}