#ifndef FIGHTER_H
#define FIGHTER_H

typedef struct fighter Fighter;

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Vector2.h"
#include "Player.h"
#include "Hitbox.h"
#include "Motion.h"
#include "Animation.h"
#include "List.h"
#include "CC_Consts.h"
#include "FighterFlags.h"
#include "Action.h"
#include "Stick.h"

typedef struct fighterState{

    Action* action;
    Animation* animation;
    Vector2 position;
    Vector2 velocity;
    // Temporary gravity, i.e. during airdash. disabled at [6969, 6969]
    Vector2 tempGravity;

    FighterFlags stateFlags;
    short health;
    short mana;
    short meter;

    unsigned char jumps;
    unsigned char frame;
    // Used for: jumpsquat
    //           hitStop
    //           tempGravity (airdash)
    unsigned char subframe;
    unsigned char gravity; // increment every fighter.gravityEvery

} FighterState;



typedef struct fighter {

    // Match constant
    uint owner;
    Fighter* opponent;
    char* name;
    uint paletteNumber;
    
    uint maxJumps;
    int maxHealth;
    int maxMana;
    int gravity;
    int maxMeter;
    Action** actions; // Stretchy buffer
    Animation** animations; // Stretchy buffer
    SDL_Texture* portrait;

    // Special move inputs, listed in order of (tiebreaking) priority
    Motion** motions; // Stretchy buffer
    FighterState* stateHistory; // Stretchy buffer

} Fighter;

void Fighter_DrawSprite(Fighter* f, SDL_Renderer* ren, SDL_Rect camrea);
void Fighter_DrawPoint(Fighter* f, SDL_Renderer* ren, SDL_Rect camrea);
void Fighter_DrawHitboxes(Fighter* f, SDL_Renderer* ren, SDL_Rect camrea);
void Fighter_DrawHurtboxes(Fighter* f, SDL_Renderer* ren, SDL_Rect camrea);
void Fighter_DrawCollisionbox(Fighter* f, SDL_Renderer* ren, SDL_Rect camrea);

void Fighter_ChangeAnimation(Fighter* f, Animation* newAnimation);

void Fighter_MoveGround(Fighter* f, StickState* ss, StickState* ess, SDL_Rect camera);
void Fighter_MoveAir(Fighter* f, StickState* ss, SDL_Rect camera);

void Fighter_Land(Fighter* f);

void Fighter_CheckWalls(Fighter* f);
void Fighter_UpdateSubstate(Fighter* f);
float Fighter_HealthPercent(Fighter* f);

void Fighter_StartAction(Fighter*f, Action* a, uint setMax);
void Fighter_StartActionIndex(Fighter*f, uint i, uint setMax);

bool Fighter_Grounded(Fighter* f);
bool Fighter_Air(Fighter* f);
bool Fighter_OnRight(Fighter* f);

// Things to free: animations (and animation->spriteClips sdl_rects)
Fighter* Fighter_Create();
void Fighter_Damage(Fighter* f, Action* a, bool onRight);
void Fighter_Free(Fighter* f);



#endif