#ifndef __UTIL_H__
#define __UTIL_H__

 #define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })


// goes through each element of the array and frees it
// finally frees the array itself
// ARRAY IS INVALID AFTER CALL TO THIS FUNCTION
// safe to pass null
extern void FreeDynamicArrayOfAllocatedElements(void** array, int numElements);

// gets the index of the last appearance of chr in the str
// if chr is not in string or NULL is passed to str, -1 is returned
extern int GetLastChar(const char* str, char chr);

/* WINDOWS: gets all the files in the dir matching the wildcard
    for example 'images/*.png' will return an array
    ["images/background.png", "images/brickwall.png", "images/grass.png"]
    returns NULL on error
   
   LINUX: gets all files in the directory. no wild cards
    for example 'images' will return an array of strings that have
    full paths (from images onwards) to all files in directory 
    ["images/diffbackground.jpg", "images/background.png", "images/brickwall.png", "images/grass.png"]
    
*/
extern char** GetAllFilesInDir(const char* dir, int* numFiles);

// path: the file path as a string
// outName: output array of function
// outSize: size of array
extern void fileNameFromPath(const char* path, char* outName, int outSize);

#endif