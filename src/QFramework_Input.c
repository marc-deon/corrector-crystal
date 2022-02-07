#ifndef QFRAMEWORK_INPUT_C
#define QFRAMEWORK_INPUT_C

#include "QFramework_Input.h"
#include <stdbool.h>

Uint8* previousKeyStates;
Uint8* currentKeyStates;
SDL_Event event;

bool QF_IsKeyDown(uint k){
    return currentKeyStates[k];
}

bool QF_IsKeyJustDown(uint k){
    return currentKeyStates[k] && !(previousKeyStates[k]);
}

bool QF_IsKeyUp(uint k){
    return !currentKeyStates[k];
}

bool QF_IsKeyJustUp(uint k){
    return !currentKeyStates[k] && previousKeyStates[k];
}

void QF_CopyKeyArray(Uint8* from, Uint8* to){
    if (from == 0) 
        return;
    for(int i = 0; i < scancodeCount; i++)
        to[i] = from[i];
}

void QF_Update(){
    currentKeyStates = (Uint8*) SDL_GetKeyboardState(NULL);
}

#endif