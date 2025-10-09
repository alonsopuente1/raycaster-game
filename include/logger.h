#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define logmaxchr 1024

#define PushLog(lvl, msg) _PushLog(lvl, msg, __LINE__, __FILE__)

#define PushLogf(lvl, format, ...) {\
    char input[logmaxchr] = {0};\
    snprintf(input, logmaxchr, format, __VA_ARGS__);\
    _PushMsg(lvl, input, __LINE__, __FILE__);\
}\

#define LogMsg(lvl, msg) _LogMsg(lvl, msg, __LINE__, __FILE__)

#define LogMsgf(lvl, format, ...) {\
    char input[logmaxchr] = {0};\
    snprintf(input, logmaxchr, format, __VA_ARGS__);\
    _LogMsg(lvl, input, __LINE__, __FILE__);\
}\



typedef enum
{
    INFO = 0,
    DEBUG = 1,
    WARN = 2,
    ERROR = 3
} errorLevel;

extern void _PushLog(errorLevel lvl, const char* msg, int line, const char* file);
extern void _LogMsg(errorLevel lvl, const char* msg, int line, const char* file);
extern void GetLog(char* buf);
extern void FreeLogs(void);
#endif
