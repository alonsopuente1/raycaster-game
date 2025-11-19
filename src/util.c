#include "util.h"

#include "logger.h"
#include <Windows.h>

void FreeDynamicArrayOfAllocatedElements(void** array, int numElements)
{
    if(!array)
        return;

    for(int i = 0; i < numElements; i++)
    {
        free(array[i]);
    }

    free(array);
}

int GetLastChar(const char* str, char chr)
{
    if(!str)
    {
        LogMsg(WARN, "null passed to str\n");
        return -1;
    }

    int lastIndex = -1;

    for(int i = 0; i < (int)strlen(str); i++)
        if(str[i] == chr)
            lastIndex = i;

    return lastIndex;
}


char** GetAllFilesInDir(const char* dir, int* numFiles)
{
    if(!dir)
    {
        LogMsg(WARN, "passed null to dir\n");
        return NULL;
    }

    if(!numFiles)
    {
        LogMsg(WARN, "passed null to numFiles\n");
        return NULL;
    }

    char**              output = NULL;
    int                 filesFound = 0;
    HANDLE              hFile = NULL;
    WIN32_FIND_DATA     fFile = { 0 };
    char                tempPath[256] = { 0 };
    int                 lastBackslashIndex = 0;
    int                 lastForwardSlashIndex = 0;
    int                 lastSlashIndex = 0;

    hFile = FindFirstFileA(dir, &fFile);       
    if(hFile == INVALID_HANDLE_VALUE)
    {
        LogMsgf(ERROR, "failed to search for files in directory '%s'\n", dir);
        return NULL;
    }

    lastBackslashIndex = GetLastChar(dir, '/');
    lastForwardSlashIndex = GetLastChar(dir, '\\');
    if(lastBackslashIndex < 0 && lastForwardSlashIndex < 0)
    {
        LogMsg(ERROR, "failed to get last backslash in dir\n");
        return NULL;
    }
    
    lastSlashIndex = lastBackslashIndex > lastForwardSlashIndex ? lastBackslashIndex : lastForwardSlashIndex;

    if(lastSlashIndex + 1 > (int)sizeof(tempPath) - 1)
    {
        LogMsg(ERROR, "length of directory too long");
        return NULL;
    }

    memcpy(tempPath, dir, lastSlashIndex + 1);

    do
    {
        char fullPath[256] = { 0 };
        memcpy(fullPath, tempPath, strlen(tempPath));

        if(strlen(fullPath) + strlen(fFile.cFileName) > sizeof(fullPath) - 1)
        {
            LogMsgf(WARN, "length of the full path is too long for destination string. skipping file '%s'\n", fFile.cFileName);
            continue;
        }

        strncat(fullPath, fFile.cFileName, strlen(fFile.cFileName));

        char** newPtr = realloc(output, sizeof(char*) * (filesFound + 1));
        if(!newPtr)
        {
            LogMsg(ERROR, "failed to allocate more memory for list of files\n");
            FreeDynamicArrayOfAllocatedElements((void**)output, filesFound);
            output = NULL;
            break;
        }
        
        output = newPtr;

        output[filesFound] = calloc(strlen(fullPath) + 1, sizeof(char));
        if(!output[filesFound])
        {
            LogMsgf(ERROR, "failed to allocate memory for file path. skipping file '%s'\n", fFile.cFileName);
            output = realloc(output, sizeof(char*) * filesFound);
            continue;
        }

        strncpy(output[filesFound], fullPath, strlen(fullPath));
        
        filesFound += 1;
        LogMsgf(DEBUG, "found file %s\n", fullPath);
    } while (FindNextFileA(hFile, &fFile));

    FindClose(hFile);

    *numFiles = filesFound;
    return output;
}

void fileNameFromPath(const char* path, char* outName, int maxLen)
{
    char* lastSlash = (char*)path;
    char* lastDot = (char*)path;
    for(char* p = (char*)path; *p != '\0'; p++)
    {
        if(*p == '/' || *p == '\\')
            lastSlash = p + 1;
        if(*p == '.')
            lastDot = p;
    }

    int charsToCopy = lastDot - lastSlash;
    if(charsToCopy >= maxLen)
        charsToCopy = maxLen - 1;
    memcpy(outName, lastSlash, charsToCopy);
    outName[charsToCopy] = '\0';
}