#ifndef PLAYER_H
#define PLAYER_H
typedef struct player Player;

#include "Stick.h"
#include "Fighter.h"

struct player {
    Stick* stick;
    Fighter* pointCharacter;
    // TODO(#4): Implement rebinding and shit
    //InputDevice device;
};

// Find the first pattern in the stick buffer supported by the point character and return its index. Returns -1 if not found.
int Player_GetFirstPattern(Player* p);

#endif /* PLAYER */
