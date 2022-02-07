#ifndef ACTION_H
#define ACTION_H

#include <stdlib.h>

#include "FighterFlags.h"
#include "Animation.h"
#include "Hitbox.h"
#include "CC_Audio.h"


/*
Everything a character does is an Action: standing, walking, blocking, attacking,
they're all actions. 99% of the time you see an animation change, 
it's because the current action changed.
*/

// typedef struct action Action;

typedef struct action {
    // Human readable name 
    char* name;  

    // Total accumulated frames since entering action
    unsigned char currentFrame; 

    int canLinkAfter;
    // Action will automatically link to another action (generally idle) after this many frames.
    // mustLinkAfter is modified by opponent in cases such as getting hit.
    // Set this to -1 if the action is intended to loop, such is idle, walking, etc.
    int mustLinkAfter;
    struct action** linksTo;
    struct action* linksFrom;

    // Upon entering the, the fighter's state will become this flag set.
    // If multiple flag sets are needed (e.g. backdash being invuln), use multiple actions.
    // No I don't know what this means lmao
    FighterFlags state;

    // Actions can be canceled into other actions with higher priority
    // e.g. jabs into straights, regular into special, hitstun into burst.
    unsigned short priority; 

    // Animation to play upon entering action
    Animation* animation; 

    // Stretchy buffer; fighter does not directly have hitboxes
    Hitbox** hitboxes;    

    // Stretchy buffer; fighter does not directly have hurtboxes
    Hurtbox** hurtboxes;  

    // AKA collision box
    Shovebox shovebox;

    // Unscaled damage, out of a default 10k health
    // Number is not used for non-attack actions, obviously
    unsigned short damage;

    // Unscaled hitstun, measured in frames
    // Number is not used for non-attack actions, obviously
    unsigned short hitstun;

    unsigned short index;

    Mix_Chunk* audioChunk;

    bool wallSplat;
    bool dunk;
    bool hardKnockdown;
    bool forceAir;

    bool overrideGravity;

    unsigned short hitStop;
    unsigned short hitStun;

    int knockback[2];
    int airKnockback[2];
    unsigned int knockbackFrames;

    int selfKnockback[2];
    unsigned int selfKnockbackFrames;

    int step[2];
    unsigned int stepDelay;

    int burnsJump;
    bool restoresJump;
    int* overrideSelfGravity;
    int* overrideSelfVelocity;
    
    // If 0, sets state to grounded; if 1, sets state to air; if -1, does not change stage
    int groundAir;

    unsigned char overrideSelfTime;

    bool phase;

} Action;

Action* Action_Create(
    char* name,
    int canLinkAfter,
    int mustLinkAfter,
    FighterFlags state,
    uint priority,
    Animation* animation,
    uint damage,
    uint hitstun,
    bool wallSplat,
    bool dunk,
    bool hardKnockdown,
    bool forceAir,
    int groundAir,
    bool overrideGravity,
    uint hitStop,
    int knockback[2],
    int airKnockback[2],
    uint knockbackFrames,
    int selfKnockback[2],
    uint selfKnockbackFrames,
    int step[2],
    uint stepDelay,
    int burnsJump,
    bool restoresJump,
    int overrideSelfGravity[2],
    int overrideSelfVelocity[2],
    uint overrideSelfTime,
    uint index,
    bool phase
    );
void Action_SetShovebox(Action*, Shovebox hb);
// Return the index of added element
int Action_AddHitbox(Action*, Hitbox* hb);
// Return the index of added element
int Action_AddHurtbox(Action*, Hurtbox* hb);
int Action_FindIndexByName(Action** a, int size, char* name);
void Action_SetLinkTo(Action* a, Action* link);
void Action_SetLinkFrom(Action* a, Action* link);
void Action_Free(Action* act);

#endif