#ifndef MOTION_H
#define MOTION_H
// #include "Animation.h"
#include "Action.h"

struct Action;
// typedef Action;

typedef struct motion{

    int priority; // Compared to animation priorty
    int airOk;
    char* name; // Human readable name
    char* pattern; // Numpad notation that must be input to match
    Action* action; // action to start if legal
    uint bufferSize; // Maximum number of frames back in the input buffer to look for this motion, -1 for infinite

} Motion;


Motion* Motion_Create(int priority, int airOk, char* name, char* pattern, Action* action, uint bufferSize);
#endif