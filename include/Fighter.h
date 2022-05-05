#ifndef FIGHTER
#define FIGHTER

typedef struct fighter Fighter;

extern float fighterDrawScale;
extern int selectedBoxIdx;
extern enum boxtype selectedBoxType;

#include "Vec2I.h"
#include "Player.h"
#include "Hitbox.h"
#include "Motion.h"
#include "Animation.h"
#include "List.h"
#include "CC_Consts.h"
#include "FighterFlags.h"
#include "Action.h"
#include "Stick.h"

// extern Shader fighterShader;

typedef struct fighterState{

    Action* action;
    Animation* animation;
    Vec2I position;
    Vec2I velocity;
    // Temporary gravity, i.e. during airdash. disabled at [6969, 6969]
    Vec2I tempGravity;

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
    Color palette[256];
    uint paletteNumber;
    Texture paletteTexture;
    Shader fighterShader;
    
    uint maxJumps;
    int maxHealth;
    int maxMana;
    int gravity;
    int maxMeter;
    Action** actions; // Stretchy buffer
    Animation** animations; // Stretchy buffer
    Texture portrait;

    // Special move inputs, listed in order of (tiebreaking) priority
    Motion** motions; // Stretchy buffer
    FighterState* stateHistory; // Stretchy buffer

} Fighter;

void Fighter_DrawSprite(Fighter* f, RectI camrea);
void Fighter_DrawPoint(Fighter* f, RectI camrea);
void Fighter_DrawHitboxes(Fighter* f, RectI camrea);
void Fighter_DrawHurtboxes(Fighter* f, RectI camrea);
void Fighter_DrawCollisionbox(Fighter* f, RectI camrea);

void Fighter_ChangeAnimation(Fighter* f, Animation* newAnimation);

void Fighter_MoveGround(Fighter* f, StickState* ss, StickState* ess, RectI camera);
void Fighter_MoveAir(Fighter* f, StickState* ss, RectI camera);

void Fighter_Land(Fighter* f);

void Fighter_CheckWalls(Fighter* f);
void Fighter_UpdateSubstate(Fighter* f);
float Fighter_HealthPercent(Fighter* f);

void Fighter_StartAction(Fighter*f, Action* a, uint setMax);
void Fighter_StartActionIndex(Fighter*f, uint i, uint setMax);

bool Fighter_Grounded(Fighter* f);
bool Fighter_Air(Fighter* f);
bool Fighter_OnRight(Fighter* f);

// Things to free: animations (and animation->spriteClips RectIs)
Fighter* Fighter_Create(char* path);
void Fighter_Damage(Fighter* f, Action* a, bool onRight);
void Fighter_Free(Fighter* f);
void Fighter_Destroy(Fighter* f);


#endif /* FIGHTER */
