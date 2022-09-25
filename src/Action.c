#include "qtypes.h"
#include "Action.h"
#include "RectI.h"
#include "stretchy_buffer.h"
#include <raylib.h>

uint _action_count;

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
    bool phase,
    int entity
){
    Action* act = (Action*) malloc(1 * sizeof(Action));
    act->currentFrame = 0;
    act->name = name;
    act->canLinkAfter = canLinkAfter;
    act->mustLinkAfter = mustLinkAfter;
    act->linksTo = 0;
    act->linksFrom = 0;
    act->state = state;
    act->priority = priority;
    act->animation = animation;
    act->hitboxes = 0;
    act->hurtboxes = 0;
    // act->shovebox;
    act->damage = damage;
    act->hitstun = hitstun;
    act->audioChunk.frameCount = 0;

    act->wallSplat = wallSplat;
    act->dunk = dunk;
    act->hardKnockdown = hardKnockdown;
    act->forceAir = forceAir;
    act->groundAir = groundAir;
    act->overrideGravity = overrideGravity;
    act->hitStop = hitStop;
    act->knockback[0] = knockback[0]; 
    act->knockback[1] = knockback[1];
    act->airKnockback[0] = airKnockback[0];
    act->airKnockback[1] = airKnockback[1];
    act->knockbackFrames = knockbackFrames;
    act->selfKnockback[0] = selfKnockback[0];
    act->selfKnockback[1] = selfKnockback[1];
    act->selfKnockbackFrames = selfKnockbackFrames;
    act->step[0] = step[0];
    act->step[1] = step[1];
    act->stepDelay = stepDelay;
    act->burnsJump = burnsJump;
    act->restoresJump = restoresJump;
    if (overrideSelfGravity){
        act->overrideSelfGravity = calloc(2, sizeof(int));
        act->overrideSelfGravity[0] = overrideSelfGravity[0];
        act->overrideSelfGravity[1] = overrideSelfGravity[1];
    } else{
        act->overrideSelfGravity = 0;
    }
    if (overrideSelfVelocity){
        act->overrideSelfVelocity = calloc(2, sizeof(int));
        act->overrideSelfVelocity[0] = overrideSelfVelocity[0];
        act->overrideSelfVelocity[1] = overrideSelfVelocity[1];
    } else{
        act->overrideSelfVelocity = 0;
    }
    act->overrideSelfTime = overrideSelfTime;
    act->index = index;
    act->phase = phase;
    act->entity = entity;
    act->cb_on_Damage.function = NULL;
    return act;
}

Action* Action_CreateNull(){
    Action* act = (Action*) malloc(1 * sizeof(Action));
    act->currentFrame = 0;
    act->name = "ACT_NONAME";
    act->canLinkAfter = 0;
    act->mustLinkAfter = -1;
    act->linksTo = 0;
    act->linksFrom = 0;
    act->state = 0;
    act->priority = 0;
    act->animation = 0;
    act->hitboxes = 0;
    act->hurtboxes = 0;
    // act->shovebox;
    act->damage = 0;
    act->hitstun = 0;
    act->audioChunk.frameCount = 0;

    act->wallSplat = 0;
    act->dunk = 0;
    act->hardKnockdown = 0;
    act->forceAir = 0;
    act->groundAir = 0;
    act->overrideGravity = 0;
    act->hitStop = 0;
    act->knockback[0] = 0;
    act->knockback[1] = 0;
    act->airKnockback[0] = 0;
    act->airKnockback[1] = 0;
    act->knockbackFrames = 0;
    act->selfKnockback[0] = 0;
    act->selfKnockback[1] = 0;
    act->selfKnockbackFrames = 0;
    act->step[0] = 0;
    act->step[1] = 0;
    act->stepDelay = 0;
    act->burnsJump = 0;
    act->restoresJump = 0;
    act->overrideSelfGravity = 0;
    act->overrideSelfVelocity = 0;
    act->overrideSelfTime = 0;
    act->index = 0;
    act->phase = 0;
    act->entity = 0;
    act->cb_on_Damage.function = NULL;
    return act;
}

void Action_Free(Action* act){
    // Mix_FreeChunk(act->audioChunk);
    free(act->overrideSelfGravity);
    free(act->overrideSelfVelocity);
    free(act);
}

int Action_FindIndexByName(Action** actions, int size, char* name){
    for(int i = 0; i < size; i++){
        int result = TextIsEqual(actions[i]->name, name);
        if(result)
            return i;
    }
    return -1;
}

void Action_SetLinkFrom(Action* a, Action* link){
    a->linksFrom = link;
}

void Action_SetShovebox(Action* a, Shovebox hb){
    a->shovebox = hb;
}

// Return the index of added element
int Action_AddHitbox(Action* a, Hitbox* hb){
    sb_push(a->hitboxes, hb);
    return sb_count(a->hitboxes)-1;
}

// Return the index of added element
int Action_AddHurtbox(Action* a, Hurtbox* hb){
    sb_push(a->hurtboxes, hb);
    return sb_count(a->hurtboxes)-1;
}
