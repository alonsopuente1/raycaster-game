#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#define TEXWIDTH 64
#define TEXHEIGHT 64

#define NUMTEXTURES 8
extern const char* gTexturePaths[NUMTEXTURES];

#define MAPWIDTH    24
#define MAPHEIGHT   24

#define MOVESPEED   0.005
#define ROTATESPEED 0.002

typedef enum
{
    false = 0,
    true = 1
} bool;

#endif