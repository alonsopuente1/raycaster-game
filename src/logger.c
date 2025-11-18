#include "logger.h"

typedef struct errorNode_s
{
    struct errorNode_s* next;
    char msg[logmaxchr];

} errorNode;

static errorNode* head = NULL;

errorNode* GetLastLogQ(void)
{
    if(!head)
    {
        return NULL;
    }

    errorNode* i = head;

    while(i->next)
    {
        i = i->next;
    }

    return i;
}

void _PushLog(errorLevel lvl, const char* msg, int line, const char* file)
{

    const char* errorlvlmsg[] = {
        "[INFO]\t\t",
        "[DEBUG]\t\t",
        "[WARN]\t\t",
        "[ERROR]\t\t"
    };

    errorNode* output = (errorNode*)malloc(sizeof(errorNode));
    memset(output, 0, sizeof(errorNode));

    if(!output)
    {
        printf("NO MEMORY. GETTING THE FUCK OUT OF HERE");
        exit(-1);
    }

    snprintf(output->msg, logmaxchr, "%sOn line %i in file %s\n\t\t%s", errorlvlmsg[(lvl)], line, file, msg);

    if(!head)
    {
        head = output;
    }
    else
    {
        errorNode* last = GetLastLogQ();
        last->next = output;
    }

    return;
}

void GetLog(char* buf)
{
    if(!head)
    {
        return;
    }

    errorNode* temp = head;
    strcpy(buf, temp->msg);
    head = head->next;
    free(temp);
}

void FreeLogs(void)
{
    if(!head)
    {
        return;
    }

    errorNode* i = head;

    while(i->next)
    {
        errorNode* temp = i;

        i = i->next;

        free(temp);
    }

    free(i);

    head = NULL;

    return;
}

void _LogMsg(errorLevel lvl, const char* msg, int line, const char* file, const char* function)
{
    const char* errorlvlmsg[] = {
        "[INFO]\t\t",
        "[DEBUG]\t\t",
        "[WARN]\t\t",
        "[ERROR]\t\t"
    };

    printf("%sAt %s:%i in function %s\n\t\t%s", errorlvlmsg[(int)lvl], file, line, function, msg);

    return;
}
