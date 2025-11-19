#ifndef __UTIL_H__
#define __UTIL_H__

// goes through each element of the array and frees it
// finally frees the array itself
// ARRAY IS INVALID AFTER CALL TO THIS FUNCTION
extern void FreeDynamicArrayOfAllocatedElements(void** array, int numElements);

// gets the index of the last appearance of chr in the str
// if chr is not in string or NULL is passed to str, -1 is returned
extern int GetLastChar(const char* str, char chr);

// TODO: implement function and add list of maps on main menu screens
extern char** GetAllFilesInDir(const char* dir, int* numFiles);

#endif