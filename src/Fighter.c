#include <raylib.h>
#include <assert.h>
#include "stretchy_buffer.h"
#include "main.h"
#include "Fighter.h"
#include "circular_buffer.h"
#include "CC_Consts.h"
#include "CC_Audio.h"
#include "Action.h"

int sign(int x){
    return x > 0 ? 1 : x < 0 ? -1 : 0;
}

Fighter* Fighter_Create(char* path){
    Fighter* f = (Fighter*)malloc(1 * sizeof(Fighter));
    f->name = "UNINITIALIZED FIGHTER";
    f->actions = NULL;
    f->animations = NULL;
    f->motions = NULL;
    f->stateHistory = NULL;
    
    Fighter_SpriteInit(f, path);

    return f;
}

bool Fighter_Grounded(Fighter* f){
    return !(cb_last(f->stateHistory).stateFlags & FF_AIR);
}

bool Fighter_Air(Fighter* f){
    return cb_last(f->stateHistory).stateFlags & FF_AIR;
}

bool Fighter_OnRight(Fighter* f){

    FighterState* sh = f->opponent->stateHistory;
    FighterState ls = cb_last(sh);

    if(cb_last(f->stateHistory).position.x > cb_last(f->opponent->stateHistory).position.x){
        return true;
    }

    else if(cb_last(f->stateHistory).position.x == cb_last(f->opponent->stateHistory).position.x){
        // In the case of a tie, place player 2 on the right.
        if(f->owner == 2){
            return true;
        }
    }
    return false;
}

void Fighter_Damage(Fighter* f, Action* a, bool onRight){
    if(!a){
        // puts("No a?");
        return;
    }

    int right = onRight ? 1 : -1;

    printf("%s hit by %s (hs:%d)\n", f->name, a->name, a->hitstun);
    
    FighterState* fs = &cb_last(f->stateHistory);
    int staggerIndex    = Action_FindIndexByName(f->actions, sb_count(f->actions), "Stagger");
    int airStaggerIndex = Action_FindIndexByName(f->actions, sb_count(f->actions), "Air Stagger");
    int hkdIndex        = Action_FindIndexByName(f->actions, sb_count(f->actions), "Back Land Hard");

    int damage = a->damage;
    if(fs->health - damage < 0){
        fs->health = 0;
    }
    else{
        fs->health -= damage;
    }
    
    if(Fighter_Air(f)){
        fs->velocity.x = a->airKnockback[0] * right;
        fs->velocity.y = a->airKnockback[1];
    }
    else{
        fs->velocity.x = a->knockback[0] * right;
        fs->velocity.y = a->knockback[1];
    }
    if(a->forceAir){
        fs->stateFlags = fs->stateFlags | FF_AIR;
    }
    if(a->hardKnockdown){
        fs->stateFlags = fs->stateFlags | FF_HARD_KD;
        // fs->subframe = a->hitStop;
    }
    if(fs->stateFlags & FF_AIR){
        Fighter_StartActionIndex(f, airStaggerIndex, a->hitstun);
    }
    else if ( fs->stateFlags & FF_HARD_KD){
        Fighter_StartActionIndex(f, hkdIndex, 0);
    }
    else{
    Fighter_StartActionIndex(f, staggerIndex, a->hitstun);
    }

}


// Use setMax for things like blockstun, hitstun, grabstun
void Fighter_StartAction(Fighter*f, Action* a, uint setMax){

    // TODO(#24): Imeplemnt step (e.g. Guile 4HK)

    FighterState* fs = &cb_last(f->stateHistory);

    fs->frame = 0;
    if (setMax > 0)
        a->mustLinkAfter = setMax;

    // Play audio
    Sound sfx = a->audioChunk;
    if(sfx.frameCount){
        CC_Audio_Play_SFX(sfx);
    }
    fs->action = a;


    for(int i = 0; i < sb_count(a->hitboxes); i++){
        a->hitboxes[i]->currentFrame = 0;
        a->hitboxes[i]->active = HB_INACTIVE;
    }

    fs->jumps -= a->burnsJump;
    if(a->restoresJump)
        fs->jumps = f->maxJumps;

    if(a->overrideSelfTime){
        fs->subframe = a->overrideSelfTime;
    }

    if (a->groundAir == 1){
        fs->stateFlags |= FF_AIR;
    }
    else if (a->groundAir == 0){
        fs->stateFlags &= ~FF_AIR;
    }


    if(a->overrideSelfVelocity){
        int right = Fighter_OnRight(f) ? -1 : 1;
        if (a->overrideSelfVelocity[0] != 6969){
            fs->velocity.x = a->overrideSelfVelocity[0] * right;
        }
        fs->velocity.y = a->overrideSelfVelocity[1];
    }

    if(a->overrideSelfGravity){
        int right = Fighter_OnRight(f) ? -1 : 1;
        fs->tempGravity.x = a->overrideSelfGravity[0] * right;
        fs->tempGravity.y = a->overrideSelfGravity[1];
    }

    Fighter_ChangeAnimation(f, a->animation);
}


void Fighter_StartActionIndex(Fighter*f, uint i, uint setMax){
    Fighter_StartAction(f, (f->actions[i]), setMax);
}


float Fighter_HealthPercent(Fighter* f){
    return (float)cb_last(f->stateHistory).health / f->maxHealth;
}

void Fighter_DrawSprite(Fighter* f, RectI camera){
    int count = cb_count(f->stateHistory);
    FighterState* fs = &(cb_last(f->stateHistory));

    Animation* curAnim = fs->animation;
    
    Animation* an = fs->animation;
    RectI** sc = fs->animation->spriteClips; 
    
    RectI* ricc = curAnim->spriteClips[fs->animation->currentFrame / curAnim->frameWait];
    
    Rectangle currentClip = {
         ricc->x,ricc->y, ricc->w, ricc->h
    };

    Vector2 target = {
        (fs->position.x - camera.x) * UNIT_TO_PIX - currentClip.width/2,
        (fs->position.y - camera.y) * UNIT_TO_PIX - currentClip.height
    };

    int flip = Fighter_OnRight(f);
    currentClip.width *= -1            * ((flip == 1) - (flip == 0));
    currentClip.x += currentClip.width * ((flip == 1));

    DrawTextureRec(fs->animation->texture, currentClip, target, WHITE);

    // Advance the frame
    assert(f && "no fighter");
    assert(f->stateHistory && "no history");
    assert(&(cb_last(f->stateHistory)) && "no 0th");

    if(!currentMatch.history->hitStop && !currentMatch.paused){
        fs->animation->currentFrame++;
    }

    // If we finish with the animation, advance to the linked animation.
    if(fs->animation->currentFrame >= fs->animation->frameWait * fs->animation->frameCount){
        Fighter_ChangeAnimation(f, fs->animation->linksTo);
    }

}

void Fighter_ChangeAnimation(Fighter* f, Animation* newAnimation){

    FighterState* fs = &cb_last(f->stateHistory);

    // Start on the loop point if new = old, otherwise start at 0.
    if(newAnimation == cb_last(f->stateHistory).animation){
        cb_last(f->stateHistory).animation->currentFrame = cb_last(f->stateHistory).animation->frameWait * newAnimation->loopStart;
    }
    else{
        cb_last(f->stateHistory).animation->currentFrame = 0;
        cb_last(f->stateHistory).animation = newAnimation;
    }
}

void Fighter_DrawPoint(Fighter* f, RectI camera){
    DrawRectangle(
        (cb_last(f->stateHistory).position.x - 1 - camera.x) * UNIT_TO_PIX,
        (cb_last(f->stateHistory).position.y - 1 - camera.y) * UNIT_TO_PIX,
        3,
        3,
        WHITE
    );
}
 
void Fighter_DrawHitboxes(Fighter* f, RectI camera){
    Action* a = cb_last(f->stateHistory).action;
    for(int i = 0; i < sb_count(a->hitboxes); i++){
        if(a->hitboxes[i]->active != HB_ACTIVE){
            continue;
        }

        RectI hbr = a->hitboxes[i]->rect;
        // Since we can't *Draw* with negative width, we need to do this
        if (Fighter_OnRight(f)){
            hbr = Rect_Flip_Draw(hbr);
        }
        DrawRectangle(
            (hbr.pos.x + cb_last(f->stateHistory).position.x - camera.x) * UNIT_TO_PIX,
            (hbr.pos.y + cb_last(f->stateHistory).position.y - camera.y) * UNIT_TO_PIX,
            hbr.size.x * UNIT_TO_PIX,
            hbr.size.y * UNIT_TO_PIX,
            (Color){255, 0, 0, 127}
        );

    }
}

void Fighter_DrawHurtboxes(Fighter* f, RectI camera){
    for(int i = 0; i < sb_count(cb_last(f->stateHistory).action->hurtboxes); i++){
        char* name = cb_last(f->stateHistory).action->name;
        // printf("%s\n", name);
        int count = sb_count(cb_last(f->stateHistory).action->hurtboxes);
        FighterState last = cb_last(f->stateHistory);
        Action* act = last.action;
        RectI hbr = act->hurtboxes[i]->rect;
        // Since we can't *Draw* with negative width, we need to do this
        if (Fighter_OnRight(f)){
            hbr = Rect_Flip_Draw(hbr);
        }
        DrawRectangle(
            (hbr.pos.x + cb_last(f->stateHistory).position.x - camera.x) * UNIT_TO_PIX,
            (hbr.pos.y + cb_last(f->stateHistory).position.y - camera.y) * UNIT_TO_PIX,
            hbr.size.x * UNIT_TO_PIX,
            hbr.size.y * UNIT_TO_PIX,
            (Color){0, 255, 0, 127}
        );
    }
}

void Fighter_DrawCollisionbox(Fighter* f, RectI camera){
    RectI hbr = cb_last(f->stateHistory).action->shovebox.rect;
    // Since we can't *Draw* with negative width, we need to do this
    if (Fighter_OnRight(f)){
        hbr = Rect_Flip_Draw(hbr);
    }
    DrawRectangle(
        (hbr.pos.x + cb_last(f->stateHistory).position.x - camera.x) * UNIT_TO_PIX,
        (hbr.pos.y + cb_last(f->stateHistory).position.y - camera.y) * UNIT_TO_PIX,
        (hbr.size.x) * UNIT_TO_PIX,
        (hbr.size.y) * UNIT_TO_PIX,
        (Color){255, 255, 255, 127}
    );
}


#define UNSHOVE 1


void Fighter_MoveGround(Fighter* f, StickState* ss, StickState* ess, RectI camera){
    ///////////////////////////////////////
    // Unshove if overlapping with opponent

    FighterState* fs = &cb_last(f->stateHistory);
    FighterState* os = &cb_last(f->opponent->stateHistory);

    RectI rect1 = Fighter_OnRight(f) ?
        Rect_Flip(fs->action->shovebox.rect)
        : fs->action->shovebox.rect;
    rect1.pos = Vec2_Add(rect1.pos, fs->position);

    RectI rect2 = Fighter_OnRight(f->opponent) ?
        Rect_Flip(cb_last(f->opponent->stateHistory).action->shovebox.rect)
        : cb_last(f->opponent->stateHistory).action->shovebox.rect;
    rect2.pos = Vec2_Add(rect2.pos, cb_last(f->opponent->stateHistory).position);


    

    int us = ss->right - ss->left;
    int opp = ess->right - ess->left;

    us = fs->velocity.x;


    // jumpsquat
    if(fs->subframe > 0){
        us = 0;
    }
    fs->stateFlags &= ~FF_HARD_KD;
    opp*=2;
    int col = Rect_ShortestEscape(rect1, rect2);

    

    // If we're not walking
    if(!(fs->action == f->actions[4] || fs->action == f->actions[5])){
        us = 0;
        opp = 0;
    }
    if(us != 0){
        fs->velocity.x = (us-(opp*col))/(1+abs(col));
    }
    else if(col){
        fs->velocity.x = UNSHOVE * abs(col) * (Fighter_OnRight(f) ? 1 : -1);
    }
    else{
        fs->velocity.x = 0;
    }
    
    // This prevents the players from moving beyond the camera
    int tentativeDistance = (fs->position.x + fs->velocity.x) - os->position.x;

    if(abs(tentativeDistance) > camera.w * PIX_TO_UNIT){
        int multiplier = fs->velocity.x > 0 ? 1 : 0;
        fs->position.x = camera.x + (multiplier * PIX_TO_UNIT * camera.w);
        fs->velocity.x = 0;
    }

    fs->position.x += fs->velocity.x;
}

void Fighter_Land(Fighter* f){
    FighterState* fs = &cb_last(f->stateHistory);

    fs->jumps = f->maxJumps;
    
    fs->stateFlags ^= FF_AIR;
    fs->velocity.x = 0;
    fs->subframe = 0;
    
    if(fs->stateFlags & FF_HARD_KD){
        int i = Action_FindIndexByName(f->actions, sb_count(f->actions), "Back Land Hard");
        Fighter_StartActionIndex(f, i, 0);
    }
    else{
        int i = Action_FindIndexByName(f->actions, sb_count(f->actions), "Land");
        Fighter_StartActionIndex(f, i, 0);
    }
}

void Fighter_MoveAir(Fighter* f, StickState* ss, RectI camera){

    FighterState* fs = &cb_last(f->stateHistory);
    FighterState* os = &cb_last(f->opponent->stateHistory);

    if(fs->tempGravity.x == 6969 && fs->tempGravity.y == 6969){
        fs->velocity.y += f->gravity;
        // Give the player a little bit of drift, as a treat.
        fs->velocity.x += (ss->right - ss->left) * 1;
    }
    else{
        fs->velocity.x += fs->tempGravity.x;
        fs->velocity.y += fs->tempGravity.y;
    }

    // This prevents the players from moving beyond the camera
    int tentativeDistance = (fs->position.x + fs->velocity.x) - os->position.x;

    if(abs(tentativeDistance) > camera.w * PIX_TO_UNIT){
        int multiplier = fs->velocity.x > 0 ? 1 : 0;
        fs->position.x = camera.x + (multiplier * PIX_TO_UNIT * camera.w);
        fs->velocity.x = 0;
    }

    fs->position = Vec2_Add(fs->position, fs->velocity);
    

    // Land if neccesary, otherwise swap to rising/falling animation if neccesary
    if (cb_last(f->stateHistory).position.y >= FLOOR_OFFSET && cb_last(f->stateHistory).velocity.y >= 0){
        cb_last(f->stateHistory).position.y = FLOOR_OFFSET;
        Fighter_Land(f);
    }

    else if (
        (cb_last(f->stateHistory).velocity.y < 0 &&               // Going upward AND
         cb_last(f->stateHistory).action == f->actions[6] &&      // Currently air idle AND
         cb_from_back(f->stateHistory, 1).action != f->actions[6] // Was not previously air idle
        )
    ){
        // Switch animation to upward loop
        Fighter_ChangeAnimation(f, f->animations[6]);
    }

    else if (
        cb_last(f->stateHistory).velocity.y >= 0 &&
        
        TextIsEqual(cb_last(f->stateHistory).animation->name, "Jump Rise")
    ){
        // Switch animation to downward loop
        // FIXME: Do this by string?
        Fighter_ChangeAnimation(f, f->animations[8]);
    }
}

void Fighter_CheckWalls(Fighter* f){
    // Collide with walls
    if(cb_last(f->stateHistory).position.x < LEFT_WALL)
        cb_last(f->stateHistory).position.x = LEFT_WALL;
    else if(cb_last(f->stateHistory).position.x > RIGHT_WALL)
        cb_last(f->stateHistory).position.x = RIGHT_WALL;
}

void Fighter_UpdateSubstate(Fighter* f){
    FighterState* fs = &cb_last(f->stateHistory);
    if(fs->subframe == 0){
        fs->tempGravity.x = 6969;
        fs->tempGravity.y = 6969;
    }
    else{
        fs->subframe--;
    }
}
