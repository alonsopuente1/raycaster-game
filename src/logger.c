#include "logger.h"

#include <time.h>

static bool set = false;
static char errorMsg[logmaxchr];

void _LogMsg(errorLevel lvl, const char* msg, int line, const char* file, const char* function)
{
    const char* errorlvlmsg[] = {
        "[INFO]\t\t",
        "[DEBUG]\t\t",
        "[WARN]\t\t",
        "[ERROR]\t\t"
    };

    printf("%sAt %s:%i in function %s\n\t\t%s\n", errorlvlmsg[(int)lvl], file, line, function, msg);

    return;
}

void SetError(const char* msg)
{
    strncpy(errorMsg, msg, logmaxchr - 1);
    errorMsg[logmaxchr - 1] = '\0';
    set = true;
}

const char* GetError()
{
    if(set)
    {
        set = false;
        return errorMsg;
    }
    else
    {
        return NULL;
    }
}

void ClearError()
{
    memset(errorMsg, 0, sizeof(errorMsg));
    set = false;
}