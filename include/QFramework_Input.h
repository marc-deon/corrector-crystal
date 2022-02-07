#ifndef QFRAMEWORK_INPUT_H
#define QFRAMEWORK_INPUT_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "qtypes.h"

// Event handler
extern SDL_Event event;

#define scancodeCount 285
extern Uint8* previousKeyStates;
extern Uint8* currentKeyStates;

bool QF_IsKeyDown(uint k);
bool QF_IsKeyJustDown(uint k);
bool QF_IsKeyUp(uint k);
bool QF_IsKeyJustUp(uint k);
void QF_CopyKeyArray(Uint8* from, Uint8* to);
void QF_Update();



#endif