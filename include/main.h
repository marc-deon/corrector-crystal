#ifndef MAIN_H
#define MAIN_H

// Standard Library includes
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

// External library includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// Local includes
#include "foo.h"
#include "Rectangle.h"
#include "Vector2.h"
#include "QFramework_Input.h"
#include "CC_Input.h"
#include "Player.h"
#include "Fighter.h"
#include "Animation.h"
#include "stretchy_buffer.h"
#include "CC_Consts.h"
#include "Match.h"

// This holds info about the window itself; size, position, fullscreen status, etc
extern SDL_Window* gWindow;
// Handles all rendering within a particular window.
extern SDL_Renderer* gRenderer;
extern Fighter p1Fighter;
extern Fighter p2Fighter;
extern SDL_Texture* background;
extern Match currentMatch;
extern bool stopGame;



// Start SDL and create window
bool CC_INIT();
// Free media and shut down SDL
void CC_CLOSE();
void ResetRound();
Match* GameInit(int p1CharaIndex, int p2CharaIndex);

#endif