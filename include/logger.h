#ifndef __LOGGER_H__
#define __LOGGER_H__

/* logger.h

    functions for logging messages in the console

    also includes a simple messagebox displaying macro

    define a pre-processor macro called LOGMSG to compile with logging
    enabled. this stops all logging in the console. messageboxes are not
    affected

    IMPORTANT:  change the below macro LOGMSGS to 1 for message logging to console to be enabled
                0 for it to be disabled
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define LOGMSGS 1

#ifdef __GNUC__
#define funcname __FUNCTION__
#else
#define funcname __func__
#endif

#define logmaxchr 1024

// these are more for displaying error msgs to user rather than logging
// so these are defined whether LOGMSGS is 1 or 0
#define ShowMessageBox(flags, title, msg)\
    SDL_ShowSimpleMessageBox(flags, title, msg, NULL);\

#define ShowMessageBoxf(flags, title, fmt, ...){\
    char input[logmaxchr] = { 0 };\
    snprintf(input, logmaxchr, fmt, __VA_ARGS__);\
    SDL_ShowSimpleMessageBox(flags, title, input, NULL);}\

#if LOGMSGS
// only works in gcc
#define LogMsg(lvl, msg) _LogMsg(lvl, msg, __LINE__, __FILE__, funcname)
#define LogMsgf(lvl, format, ...) {\
    char input[logmaxchr] = {0};\
    snprintf(input, logmaxchr, format, __VA_ARGS__);\
    _LogMsg(lvl, input, __LINE__, __FILE__, funcname);\
}\

#define SetErrorf(fmt, ...) {\
    char input[logmaxchr] = { 0 };\
    snprintf(input, logmaxchr, fmt, __VA_ARGS__);\
    SetError(input);\
}\

#else

#define LogMsg(lvl, msg)
#define LogMsgf(lvl, format, ...)

#endif

typedef enum
{
    INFO = 0,
    DEBUG = 1,
    WARN = 2,
    ERROR = 3
} errorLevel;

extern void _LogMsg(errorLevel lvl, const char* msg, int line, const char* file, const char* function);
extern void SetError(const char* msg);
// returns the pointer to internal error message
// error message 
extern const char* GetError();
extern void ClearError();

#endif
