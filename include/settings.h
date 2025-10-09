#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#define TEXWIDTH 64
#define TEXHEIGHT 64

static const char* gTexturePaths[] = {
    "res/textures/env/bluestone.png",
    "res/textures/env/colorstone.png",
    "res/textures/env/greystone.png",
    "res/textures/env/mossy.png",
    "res/textures/env/purplestone.png",
    "res/textures/env/redbrick.png",
    "res/textures/env/wood.png",
    "res/textures/env/eagle.png",
    "res/textures/player.png",
    "res/textures/guns/PIST2.png"
};

#define NUMTEXTURES ((int)(sizeof(gTexturePaths) / sizeof(gTexturePaths[0])))

#define MOVESPEED   0.005
#define ROTATESPEED 0.002

typedef enum
{
    false = 0,
    true = 1
} bool;

#endif