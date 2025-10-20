#include "m_game.h"

#include "logger.h"

#ifdef linux
#error linux support not implemented yet
#endif

maingame_t game = { 0 };

int main(int argc, char** argv)
{
    LogMsg(DEBUG, "Starting RayCaster...\n");

    G_Init(&game);

    G_Run(&game);

    G_Destroy(&game);

    return 0;
}