// SEE: https://lazyfoo.net/tutorials/SDL/18_key_states/index.php and https://lazyfoo.net/tutorials/SDL/19_gamepads_and_joysticks/index.php

#include <stdio.h>
#include "CC_Input.h"
#include "QFramework_Input.h"
#include <raylib.h>

Stick p1Stick = {.playerNumber = 1};
Stick p2Stick = {.playerNumber = 2};



void SaveInput(){
    // Save Old
    QF_CopyKeyArray(currentKeyStates, previousKeyStates);
}


void UpdateInput(){
    // If input breaks, try uncommenting this.
    // SDL_PollEvent(event);

    // Get New
    QF_Update();
    
    StickState p1State = {
        QF_IsKeyDown(KEY_W) & !QF_IsKeyDown(KEY_S),
        QF_IsKeyDown(KEY_S) & !QF_IsKeyDown(KEY_W),
        QF_IsKeyDown(KEY_A) & !QF_IsKeyDown(KEY_D),
        QF_IsKeyDown(KEY_D) & !QF_IsKeyDown(KEY_A),
        QF_IsKeyDown(KEY_J),
        QF_IsKeyDown(KEY_K),
        QF_IsKeyDown(KEY_L),
        QF_IsKeyDown(KEY_U),
        QF_IsKeyDown(KEY_I),
        QF_IsKeyDown(KEY_O),
        QF_IsKeyDown(KEY_FIVE),
        QF_IsKeyDown(KEY_ONE),
    };
    Stick_AddState(&p1Stick, p1State);

    StickState p2State = {
        QF_IsKeyDown(KEY_UP)    & !QF_IsKeyDown(KEY_DOWN),
        QF_IsKeyDown(KEY_DOWN)  & !QF_IsKeyDown(KEY_UP),
        QF_IsKeyDown(KEY_LEFT)  & !QF_IsKeyDown(KEY_RIGHT),
        QF_IsKeyDown(KEY_RIGHT) & !QF_IsKeyDown(KEY_LEFT),
        QF_IsKeyDown(KEY_KP_4),
        QF_IsKeyDown(KEY_KP_5),
        QF_IsKeyDown(KEY_KP_6),
        QF_IsKeyDown(KEY_KP_7),
        QF_IsKeyDown(KEY_KP_8),
        QF_IsKeyDown(KEY_KP_9),
        QF_IsKeyDown(KEY_SIX),
        QF_IsKeyDown(KEY_TWO),
    };
    Stick_AddState(&p2Stick, p2State);

    SaveInput();

}
