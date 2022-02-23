#ifndef QFRAMEWORK_INPUT_H
#define QFRAMEWORK_INPUT_H

#include <stdbool.h>
#include <stdint.h>
#include "qtypes.h"


#define scancodeCount 285
extern uint8_t* previousKeyStates;
extern uint8_t* currentKeyStates;

bool QF_IsKeyDown(uint k);
bool QF_IsKeyJustDown(uint k);
bool QF_IsKeyUp(uint k);
bool QF_IsKeyJustUp(uint k);
void QF_CopyKeyArray(uint8_t* from, uint8_t* to);
void QF_Update();



#endif /* QFRAMEWORK_INPUT */
