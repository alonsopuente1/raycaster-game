#ifndef __S_GAMEEVENTS_H__
#define __S_GAMEEVENTS_H__

/* s_gameevents.h

    contains user defined SDL events for certain events
    the description for the data in each event will be in a 
    comment above the respective event

*/

typedef enum userevents_e
{
    // data1 -  contains a dynamically allocated string to the file path
    //          of the map to load. handled in the game scene
    EVENT_LOADMAP

} userevents_t;

#endif