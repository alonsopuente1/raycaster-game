#ifndef __ERROR_H__
#define __ERROR_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* logger.h

    functions for logging messages in the console

    define a pre-processor macro called LOGMSG to compile with logging
    enabled

*/

#ifdef __GNUC__
#define funcname __FUNCTION__
#else
#define funcname __func__
#endif

#define logmaxchr 1024

#define PushLog(lvl, msg) _PushError(lvl, msg, __LINE__, __FILE__)

#define PushLogf(lvl, format, ...) {\
    char input[logmaxchr] = {0};\
    snprintf(input, logmaxchr, format, __VA_ARGS__);\
    _PushMsg(lvl, input, __LINE__, __FILE__);\
}\


#ifdef LOGMSGS
// only works in gcc
#define LogMsg(lvl, msg) _LogMsg(lvl, msg, __LINE__, __FILE__, funcname)
#define LogMsgf(lvl, format, ...) {\
    char input[logmaxchr] = {0};\
    snprintf(input, logmaxchr, format, __VA_ARGS__);\
    _LogMsg(lvl, input, __LINE__, __FILE__, __FUNCTION__);\
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

extern void _PushLog(errorLevel lvl, const char* msg, int line, const char* file);
extern void _LogMsg(errorLevel lvl, const char* msg, int line, const char* file, const char* function);
extern void GetLog(char* buf);
extern void FreeLogs(void);
#endif
