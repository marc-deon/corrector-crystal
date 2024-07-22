#include <raylib.h>
#include <stdio.h>
#include "Action.h"
#include "CC_Audio.h"
#include "CC_Consts.h"
#include "circular_buffer.h"
#include "Entities.h"
#include "Entity.h"
#include "Fighter_Read.h"
#include "Fighter.h"
#include "Match.h"
#include "stretchy_buffer.h"

extern Match currentMatch;
extern int asamiya;

float fighterDrawScale = 1;


int selectedBoxIdx = -1;
enum boxtype selectedBoxType = boxtype_none;

inline int sign(int x) {
    return x > 0 ? 1 : x < 0 ? -1 : 0;
}


Fighter* Fighter_Create(char* path) {
    Fighter* f = (Fighter*)malloc(1 * sizeof(Fighter));
    Entity* e = Entity_Create(NULL);
    e->fighter = f;
    f->entity = e;
    e->name = "UNINITIALIZED FIGHTER";
    f->actions = NULL;
    f->animations = NULL;
    f->motions = NULL;
    f->stateHistory = NULL;
    f->opponent = NULL;

    cb_init(f->stateHistory, MAX_REWIND);
    

    Fighter_SpriteInit(f, path);
    
    e->shader = LoadShader(0, "testshader.fs");

    return f;
}

bool Fighter_Grounded(Fighter* f) {
    return !(cb_last(f->stateHistory).stateFlags & FF_AIR);
}

bool Fighter_Air(Fighter* f) {
    return cb_last(f->stateHistory).stateFlags & FF_AIR;
}

bool Fighter_FacingRight(Fighter* f) {
    return Entity_FacingRight(f->entity);
}

bool Fighter_OnRight(Fighter* f) {

    if (!f->opponent)
        return false;

    FighterState* sh = f->opponent->stateHistory;
    FighterState ls = cb_last(sh);

    if(cb_last(f->entity->history).position.x > cb_last(f->opponent->entity->history).position.x) {
        return true;
    }

    else if(cb_last(f->entity->history).position.x == cb_last(f->opponent->entity->history).position.x) {
        // In the case of a tie, place player 2 on the right.
        if(f->ownerIndex == 2) {
            return true;
        }
    }
    return false;
}

void Fighter_Damage(Fighter* f, Action* a) {
    if(!a) {
        return;
    }

    
    FighterState* fs = &cb_last(f->stateHistory);
    EntityState* es = &cb_last(f->entity->history);
    int right = es->facingRight ? 1 : -1;
    int staggerIndex    = Action_FindIndexByName(f->actions, sb_count(f->actions), "Stagger");
    int airStaggerIndex = Action_FindIndexByName(f->actions, sb_count(f->actions), "Air Stagger");
    int hkdIndex        = Action_FindIndexByName(f->actions, sb_count(f->actions), "Back Land Hard");
    
    if(Fighter_Air(f)) {
        es->velocity.x = a->airKnockback[0] * right * -1;
        es->velocity.y = a->airKnockback[1];
    }
    else {
        es->velocity.x = a->knockback[0] * right * -1;
        es->velocity.y = a->knockback[1];
    }
    if(a->forceAir) {
        fs->stateFlags = fs->stateFlags | FF_AIR;
    }
    if(a->hardKnockdown) {
        fs->stateFlags = fs->stateFlags | FF_HARD_KD;
    }
    if(fs->stateFlags & FF_AIR) {
        Fighter_StartActionIndex(f, airStaggerIndex, a->hitstun);
    }
    else if ( fs->stateFlags & FF_HARD_KD) {
        Fighter_StartActionIndex(f, hkdIndex, 0);
    }
    else{
    Fighter_StartActionIndex(f, staggerIndex, a->hitstun);
    }

}


// Use setMax for things like blockstun, hitstun, grabstun
void Fighter_StartAction(Fighter* f, Action* a, uint setMax) {

    FighterState* fs = &cb_last(f->stateHistory);
    
    // TODO: Some entities shouldn't spawn immediately
    // Probably don't worry about that here, but add it to a queue within Entities_SpawnType
    if (a->partial_entity && !asamiya) {
        Entity* partial = Entity_Copy(a->partial_entity);
        Entity_Initialize_Partial(partial, f->entity);

    }

    fs->frame = 0;
    
    fs->jumps -= a->burnsJump;
    if(a->restoresJump)
        fs->jumps = f->maxJumps;

    if (a->groundAir == 1) {
        fs->stateFlags |= FF_AIR;
    }
    else if (a->groundAir == 0) {
        fs->stateFlags &= ~FF_AIR;
    }

    if(a->overrideSelfGravity) {
        int right = Fighter_FacingRight(f) ? 1 : -1;
        fs->tempGravity.x = a->overrideSelfGravity[0] * right;
        fs->tempGravity.y = a->overrideSelfGravity[1];
    }

    Entity_StartAction(f->entity, a, setMax);

    // Fighter_ChangeAnimation(f, a->animation);
}


void Fighter_StartActionIndex(Fighter*f, uint i, uint setMax) {
    Fighter_StartAction(f, f->actions[i], setMax);
}


float Fighter_HealthPercent(Fighter* f) {
    return (float) cb_last(f->entity->history).currentHealth / f->maxHealth;
}

void Fighter_DrawSprite(Fighter* f, RectI camera) {
    Entity_DrawSprite(f->entity, camera);
}


void Fighter_DrawPoint(Fighter* f, RectI camera) {
    EntityState* es = &cb_last(f->entity->history);
    DrawRectangle(
        (es->position.x * fighterDrawScale - 1 - camera.x) * UNIT_TO_PIX,
        (es->position.y * fighterDrawScale - 1 - camera.y) * UNIT_TO_PIX,
        3,
        3,
        WHITE
    );
}
 

void Fighter_DrawHitboxes(Fighter* f, RectI camera) {
    Entity_DrawHitboxes(f->entity, camera);
}

void Fighter_DrawBlockboxes(Fighter* f, RectI camera) {
    Entity_DrawBlockboxes(f->entity, camera);
}

void Fighter_DrawHurtboxes(Fighter* f, RectI camera) {
    EntityState* es = &cb_last(f->entity->history);

    for(int i = 0; i < sb_count(es->currentAction->hurtboxes); i++) {
        char* name = es->currentAction->name;
        RectI hbr = es->currentAction->hurtboxes[i]->rect;
        // Since we can't *Draw* with negative width, we need to do this
        if (Fighter_FacingRight(f)) {
            hbr = Rect_Flip_Draw(hbr);
        }

        int selected = selectedBoxType == boxtype_none || (selectedBoxType == boxtype_hurt && (selectedBoxIdx == i || selectedBoxIdx < 0));
        int opac = 127 * selected + 64 * !selected;
        opac += 100 * (selectedBoxIdx == i && selectedBoxType == boxtype_hurt);

        DrawRectangle(
            ((hbr.pos.x + es->position.x) * fighterDrawScale - camera.x) * UNIT_TO_PIX,
            ((hbr.pos.y + es->position.y) * fighterDrawScale - camera.y) * UNIT_TO_PIX,
            hbr.size.x * UNIT_TO_PIX * fighterDrawScale,
            hbr.size.y * UNIT_TO_PIX * fighterDrawScale,
            (Color) {0, 255, 0, opac}
        );
    }
}

void Fighter_DrawCollisionbox(Fighter* f, RectI camera) {
    EntityState* es = &cb_last(f->entity->history);
    RectI hbr = es->currentAction->shovebox.rect;
    // Since we can't *Draw* with negative width, we need to do this
    if (Fighter_FacingRight(f)) {
        hbr = Rect_Flip_Draw(hbr);
    }

    int selected = selectedBoxType == boxtype_none || selectedBoxType == boxtype_shove;
    int opac = 127 * selected + 64 * !selected;
    opac += 100 * selectedBoxType == boxtype_shove;

    DrawRectangle(
        ((hbr.pos.x + es->position.x) * fighterDrawScale - camera.x) * UNIT_TO_PIX,
        ((hbr.pos.y + es->position.y) * fighterDrawScale - camera.y) * UNIT_TO_PIX,
        (hbr.size.x) * UNIT_TO_PIX * fighterDrawScale,
        (hbr.size.y) * UNIT_TO_PIX * fighterDrawScale,
        (Color) {255, 255, 255, opac}
    );
}


#define UNSHOVE 1


void Fighter_Process_Advance(Fighter* f) {
    FighterState* fs = &cb_last(f->stateHistory);
    fs->frame++;

    Entity_Process_Advance(f->entity);

    EntityState* es = &cb_last(f->entity->history);

    if(es->frame == es->currentAction->mustLinkAfter) {
        // if(fs->stateFlags & FF_HARD_KD) {
        //     Entity_StartAction(e, es->currentAction, 0);
        // }
        // else{
            Entity_StartAction(f->entity, es->currentAction->linksTo[0], 0);
        // }
    }    
    else if(es->currentAction->mustLinkAfter >= 0 && es->frame > es->currentAction->mustLinkAfter) {
        printf("%s: %d >%d\n", es->currentAction->name, es->frame, es->currentAction->mustLinkAfter);
        printf("This shouldn't happen. %s (%d)\n", __FILE__, __LINE__);
        // return false;
    }
}

void Fighter_MoveGround(Fighter* f, StickState* ss, StickState* ess, RectI camera) {
    ///////////////////////////////////////
    // Unshove if overlapping with opponent

    FighterState* fs = &cb_last(f->stateHistory);
    FighterState* os = &cb_last(f->opponent->stateHistory);
    EntityState* es = &cb_last(f->entity->history);
    EntityState* oes = &cb_last(f->opponent->entity->history);

    es->facingRight = !Fighter_OnRight(f);

    RectI rect1 = Fighter_OnRight(f) ? Rect_Flip(es->currentAction->shovebox.rect) : es->currentAction->shovebox.rect;
    rect1.pos = Vec2_Add(rect1.pos, es->position);

    RectI rect2 = Fighter_OnRight(f->opponent) ? Rect_Flip(oes->currentAction->shovebox.rect) : oes->currentAction->shovebox.rect;
    rect2.pos = Vec2_Add(rect2.pos, oes->position);


    
    int us = es->velocity.x;
    int opp = ess->right - ess->left;

    // jumpsquat
    if(es->subframe > 0) {
        us = 0;
    }
    fs->stateFlags &= ~FF_HARD_KD;
    opp*=2;
    int col = Rect_ShortestEscape(rect1, rect2);

    

    // If we're not walking
    if(!(es->currentAction == f->actions[4] || es->currentAction == f->actions[5])) {
        us = 0;
        opp = 0;
    }

    // we're walking
    if(us != 0) {
        es->velocity.x = (us-(opp*col))/(1+abs(col));
    }
    // we're stationary, opponent is walking into us
    else if(col) {
        es->velocity.x = UNSHOVE * abs(col) * (Fighter_OnRight(f) ? 1 : -1);
    }
    else{
        es->velocity.x = 0;
    }
    
    // This prevents the players from moving beyond the camera
    int tentativeDistance = (es->position.x + es->velocity.x) - oes->position.x;

    if(abs(tentativeDistance) > camera.w * PIX_TO_UNIT) {
        int multiplier = es->velocity.x > 0 ? 1 : 0;
        es->position.x = camera.x + (multiplier * PIX_TO_UNIT * camera.w);
        es->velocity.x = 0;
    }

    // es->position.x += es->velocity.x;
    Entity_Process_Position(f->entity);    
}

void Fighter_Land(Fighter* f) {
    FighterState* fs = &cb_last(f->stateHistory);
    EntityState* es = &cb_last(f->entity->history);

    fs->jumps = f->maxJumps;
    
    fs->stateFlags ^= FF_AIR;
    es->velocity = VEC2_ZERO;
    
    es->subframe = 0;
    
    if(fs->stateFlags & FF_HARD_KD) {
        int i = Action_FindIndexByName(f->actions, sb_count(f->actions), "Back Land Hard");
        Fighter_StartActionIndex(f, i, 0);
    }
    else{
        int i = Action_FindIndexByName(f->actions, sb_count(f->actions), "Land");
        Fighter_StartActionIndex(f, i, 0);
    }
}

void Fighter_MoveAir(Fighter* f, StickState* ss, RectI camera) {

    FighterState* fs = &cb_last(f->stateHistory);
    EntityState* es = &cb_last(f->entity->history);
    FighterState* os = &cb_last(f->opponent->stateHistory);
    EntityState* oes = &cb_last(f->opponent->entity->history);

    if(fs->tempGravity.x == 6969 && fs->tempGravity.y == 6969) {
        es->velocity.y += f->gravity;
        // Give the player a little bit of drift, as a treat.
        es->velocity.x += (ss->right - ss->left) * 1;
    }
    else{
        es->velocity.x += fs->tempGravity.x;
        es->velocity.y += fs->tempGravity.y;
    }

    // This prevents the players from moving beyond the camera
    int tentativeDistance = (es->position.x + es->velocity.x) - oes->position.x;

    if(abs(tentativeDistance) > camera.w * PIX_TO_UNIT) {
        int multiplier = es->velocity.x > 0 ? 1 : 0;
        es->position.x = camera.x + (multiplier * PIX_TO_UNIT * camera.w);
        es->velocity.x = 0;
    }

    Entity_Process_Position(f->entity);    

    // Land if neccesary, otherwise swap to rising/falling animation if neccesary
    if (es->position.y >= FLOOR_OFFSET && es->velocity.y >= 0) {
        es->position.y = FLOOR_OFFSET;
        Fighter_Land(f);
    }

    else if (
        (es->velocity.y < 0 &&                  // Going upward AND
         es->currentAction == f->actions[6] &&  // Currently air idle AND
         cb_from_back(f->entity->history, 1).currentAction != f->actions[6] // Was not previously air idle
        )
    ) {
        // Switch animation to upward loop
        Entity_ChangeAnimation(f->entity, f->animations[6]);
    }

    else if (
        es->velocity.y >= 0 && TextIsEqual(es->currentAnimation->name, "Jump Rise")) {
        // Switch animation to downward loop
        // FIXME: Do this by string?
        Entity_ChangeAnimation(f->entity, f->animations[8]);
    }
}

void Fighter_CheckWalls(Fighter* f) {
    EntityState* es = &cb_last(f->entity->history);
    if(es->position.x < LEFT_WALL)
        es->position.x = LEFT_WALL;
    else if(es->position.x > RIGHT_WALL)
        es->position.x = RIGHT_WALL;
}

void Fighter_UpdateSubstate(Fighter* f) {
    FighterState* fs = &cb_last(f->stateHistory);
    EntityState* es = &cb_last(f->entity->history);
    if(es->subframe == 0) {
        fs->tempGravity.x = 6969;
        fs->tempGravity.y = 6969;
    }
    else{
        es->subframe--;
    }
}
