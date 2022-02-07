// SEE: https://lazyfoo.net/tutorials/SDL/18_key_states/index.php and https://lazyfoo.net/tutorials/SDL/19_gamepads_and_joysticks/index.php

#ifndef CC_INPUT_C
#define CC_INPUT_C
#include <stdio.h>
#include <SDL2/SDL.h>
#include "CC_Input.h"
#include "QFramework_Input.h"

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
        QF_IsKeyDown(SDL_SCANCODE_W) & !QF_IsKeyDown(SDL_SCANCODE_S),   // Don't allow left+right or up+down inputs
        QF_IsKeyDown(SDL_SCANCODE_S) & !QF_IsKeyDown(SDL_SCANCODE_W),   // Don't allow left+right or up+down inputs
        QF_IsKeyDown(SDL_SCANCODE_A) & !QF_IsKeyDown(SDL_SCANCODE_D),   // Don't allow left+right or up+down inputs
        QF_IsKeyDown(SDL_SCANCODE_D) & !QF_IsKeyDown(SDL_SCANCODE_A),   // Don't allow left+right or up+down inputs
        QF_IsKeyDown(SDL_SCANCODE_J),
        QF_IsKeyDown(SDL_SCANCODE_K),
        QF_IsKeyDown(SDL_SCANCODE_L),
        QF_IsKeyDown(SDL_SCANCODE_U),
        QF_IsKeyDown(SDL_SCANCODE_I),
        QF_IsKeyDown(SDL_SCANCODE_O),
        QF_IsKeyDown(SDL_SCANCODE_RETURN),
        QF_IsKeyDown(SDL_SCANCODE_BACKSPACE),
    };
    Stick_AddState(&p1Stick, p1State);

    StickState p2State = {
        QF_IsKeyDown(SDL_SCANCODE_UP)    & !QF_IsKeyDown(SDL_SCANCODE_DOWN),    // Don't allow left+right or up+down inputs
        QF_IsKeyDown(SDL_SCANCODE_DOWN)  & !QF_IsKeyDown(SDL_SCANCODE_UP),      // Don't allow left+right or up+down inputs
        QF_IsKeyDown(SDL_SCANCODE_LEFT)  & !QF_IsKeyDown(SDL_SCANCODE_RIGHT),   // Don't allow left+right or up+down inputs
        QF_IsKeyDown(SDL_SCANCODE_RIGHT) & !QF_IsKeyDown(SDL_SCANCODE_LEFT),    // Don't allow left+right or up+down inputs
        QF_IsKeyDown(SDL_SCANCODE_KP_4),
        QF_IsKeyDown(SDL_SCANCODE_KP_5),
        QF_IsKeyDown(SDL_SCANCODE_KP_6),
        QF_IsKeyDown(SDL_SCANCODE_KP_7),
        QF_IsKeyDown(SDL_SCANCODE_KP_8),
        QF_IsKeyDown(SDL_SCANCODE_KP_9),
        QF_IsKeyDown(SDL_SCANCODE_KP_PLUS),
        QF_IsKeyDown(SDL_SCANCODE_KP_MINUS),
    };
    Stick_AddState(&p2Stick, p2State);

    SaveInput();

}


#endif