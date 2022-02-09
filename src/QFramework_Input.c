#include "QFramework_Input.h"
#include <stdbool.h>
#include <stdint.h>

uint8_t* previousKeyStates;
uint8_t* currentKeyStates;
// SDL_Event event;

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

void QF_CopyKeyArray(uint8_t* from, uint8_t* to){
    if (from == 0) 
        return;
    for(int i = 0; i < scancodeCount; i++)
        to[i] = from[i];
}

void QF_Update(){
    // currentKeyStates = (uint8_t*) SDL_GetKeyboardState(NULL);
}