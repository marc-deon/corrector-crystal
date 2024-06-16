#ifndef TYPES_H
#define TYPES_H
typedef unsigned int uint;

typedef struct CallbackInfo {
    int player;
    void* target; // UiElement*
    int arg;
} CallbackInfo;

typedef struct CallbackFunction {
    void (*function)();
    CallbackInfo info;
} CallbackFunction;

#endif