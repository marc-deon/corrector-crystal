#ifndef MAIN
#define MAIN

// Standard Library includes
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

// External library includes
#include <raylib.h>

// Local includes
#include "RectI.h"
#include "Vec2I.h"
#include "QFramework_Input.h"
#include "CC_Input.h"
#include "Player.h"
#include "Fighter.h"
#include "Animation.h"
#include "stretchy_buffer.h"
#include "CC_Consts.h"
#include "Match.h"

extern Fighter p1Fighter;
extern Fighter p2Fighter;
extern Texture background;
extern Match currentMatch;
extern bool stopGame;



// Start SDL and create window
bool CC_INIT();
// Free media and shut down SDL
void CC_CLOSE();
void ResetRound();
Match* GameInit(int p1CharaIndex, int p2CharaIndex);

#endif /* MAIN */
