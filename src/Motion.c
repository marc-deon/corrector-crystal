#include "Motion.h"

Motion* Motion_Create(int priority, int airOk, char* name, char* pattern, Action* action, uint bufferSize) {
    Motion* m = calloc(1, sizeof(Motion));
    m->priority = priority;
    m->airOk = airOk;
    m->name = name;
    m->pattern = pattern;
    m->action = action;
    m->bufferSize = bufferSize;
    return m;
}
