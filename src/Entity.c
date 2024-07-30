#include "Entity.h"
#include "stretchy_buffer.h"
#include "circular_buffer.h"
#include "stdio.h"
#include "Fighter.h"
#include "Match.h"
#include <string.h>

#include <stdio.h>

extern Match currentMatch;

extern float fighterDrawScale;
extern int selectedBoxIdx;
extern enum boxtype selectedBoxType;


EntityState* EntityState_Create() {
    EntityState* es = malloc(sizeof(EntityState));
    es->valid = false;
    es->position = (Vec2I) {0,0};
    es->velocity = (Vec2I) {0,0};
    es->facingRight = true;

    es->currentAction = NULL;
    es->currentAnimation = NULL;

    es->currentHealth = 0;

    es->frame = 0;
    es->subframe = 0;
    return es;
}


Entity* Entity_Create(Entity* owner) {
    Entity* e = malloc(sizeof(Entity));
    EntityState* es = malloc(sizeof(EntityState));
    /////
    // Entity setup
    e->name = NULL;
    e->owner = owner;
    e->fighter = NULL;
    if(owner)
        sb_push(e->owner->subEntities, e);

    e->history          = NULL;  // Circular
    e->subEntities      = NULL;  // Stretchy
    es->currentAction   = NULL;
    es->currentAnimation= NULL;
    cb_init(e->history, MAX_REWIND);

    /////
    // Entity state setup
    es->valid = false;
    es->velocity = VEC2_ZERO;
    es->currentAction = NULL;
    es->currentAnimation = NULL;
    es->currentHealth = 0;
    es->frame = 0;
    es->subframe = 0;
    
    return e;
}


bool Entity_FacingRight(Entity* e) {
    return cb_last(e->history).facingRight;
}


void Entity_DrawSprite(Entity* e, RectI camera) {
    
    EntityState* es = &cb_last(e->history);
    if (!es->valid) {
        // printf("EntityState invalid\n");
        return;
    }

    Animation* animation = es->currentAnimation;
    
    if (!animation) {
        // printf("Animation invalid\n");
        return;
    }

    RectI** sc = animation->spriteClips; 
    
    RectI* ricc = animation->spriteClips[animation->currentFrame / animation->frameWait];
    
    Rectangle currentClip = { ricc->x, ricc->y, ricc->w, ricc->h };

    int flip = !es->facingRight;
    currentClip.width *= !flip - flip;
    currentClip.x += currentClip.width * flip;
    
    Rectangle targetRect = {
        .x = (es->position.x - camera.x) * UNIT_TO_PIX - abs(currentClip.width)/2 * fighterDrawScale,
        .y = (es->position.y - camera.y) * UNIT_TO_PIX - abs(currentClip.height) * fighterDrawScale,
        .width  = abs(currentClip.width) * fighterDrawScale,
        .height = abs(currentClip.height) * fighterDrawScale
    };


    // Following 1 line temp commented
    if (e->shader.id) {
        BeginShaderMode(e->shader);
    }
    // Draw a part of a texture defined by a rectangle with 'pro' parameters
    DrawTexturePro(
        animation->texture, // Texture
        currentClip,        // Source rect
        targetRect,         // Destination rect
        (Vector2) {0,0},    // Origin
        0,                  // Rotation
        WHITE               // Tint
    );
    if (e->shader.id) {
        EndShaderMode();
    }

    // Advance the frame
    if(!cb_last(currentMatch.history).hitStop && !currentMatch.paused) {
        animation->currentFrame++;
    }

    // If we finish with the animation, advance to the linked animation.
    if(animation->currentFrame >= animation->frameWait * animation->frameCount) {
        Entity_ChangeAnimation(e, animation->linksTo);
    }


    for(int i = 0; i < sb_count(e->subEntities); i++) {
        Entity_DrawSprite(e->subEntities[i], camera);
    }
}


void Entity_Move(Entity* e) {
    cb_last(e->history).position = Vec2_Add(cb_last(e->history).position, cb_last(e->history).velocity);
}


void Entity_ChangeAnimation(Entity* e, Animation* newAnimation) {

    EntityState* es = &cb_last(e->history);

    // Start on the loop point if new = old, otherwise start at 0.
    if(newAnimation == es->currentAnimation) {
        es->currentAnimation->currentFrame = es->currentAnimation->frameWait * newAnimation->loopStart;
    }
    else{
        es->currentAnimation = newAnimation;
        es->currentAnimation->currentFrame = 0;
    }
}


void Entity_StartAction(Entity* e, Action* a, int setMax) {
    // Play audio
    Sound sfx = a->audioChunk;
    if(sfx.frameCount)
        CC_Audio_Play_SFX(sfx);


    EntityState* es = &cb_last(e->history);
    es->frame = 0;
    es->currentAction = a;

    if (setMax > 0)
        a->mustLinkAfter = setMax;
    
    if(a->overrideSelfTime)
        es->subframe = a->overrideSelfTime;
    
    
    if(a->overrideSelfTime)
        es->subframe = a->overrideSelfTime;

    if(a->overrideSelfVelocity) {
        int right = Entity_FacingRight(e) ? 1 : -1;
        if (a->overrideSelfVelocity[0] != 6969) {
            es->velocity.x = a->overrideSelfVelocity[0] * right;
        }
        es->velocity.y = a->overrideSelfVelocity[1];
    }

    for(int i = 0; i < sb_count(a->hitboxes); i++) {
        a->hitboxes[i]->currentFrame = 0;
        a->hitboxes[i]->active = HB_INACTIVE;
    }

    for(int i = 0; i < sb_count(a->blockboxes); i++) {
        a->blockboxes[i]->currentFrame = 0;
        a->blockboxes[i]->active = HB_INACTIVE;
    }

    Entity_ChangeAnimation(e, a->animation);
}


void Entity_Process_Advance(Entity* e) {
    EntityState* es  = &cb_last(e->history);
    es->frame++;


    for(int i = 0; i < sb_count(e->subEntities); i++)
        Entity_Process_Advance(e->subEntities[i]);
}


void Entity_Process_Position(Entity* e) {
    EntityState* es  = &cb_last(e->history);
    es->position = Vec2_Add(es->position, es->velocity);
    for(int i = 0; i < sb_count(e->subEntities); i++)
        Entity_Process_Position(e->subEntities[i]);
}


void Entity_Process_Hitbox(Entity* e) {
        Hitbox** hitboxes = cb_last(e->history).currentAction->hitboxes;
        Blockbox** blockboxes = cb_last(e->history).currentAction->blockboxes;

        for(int hit = 0; hit < sb_count(hitboxes); hit++) {
            Hitbox_UpdateTimer(hitboxes[hit]);
        }

        for(int hit = 0; hit < sb_count(blockboxes); hit++) {
            Hitbox_UpdateTimer(blockboxes[hit]);
        }

        for(int i = 0; i < sb_count(e->subEntities); i++) {
            Entity_Process_Hitbox(e->subEntities[i]);
        }
}

void Entity_DrawHitboxes(Entity* e, RectI camera) {
    EntityState* es = &cb_last(e->history);
    Action* a = es->currentAction;

    for(int i = 0; i < sb_count(a->hitboxes); i++) {
        if(a->hitboxes[i]->active != HB_ACTIVE) {
            continue;
        }

        RectI hbr = a->hitboxes[i]->rect;
        // Since we can't *Draw* with negative width, we need to do this
        if (!Entity_FacingRight(e))
            hbr = Rect_Flip_Draw(hbr);

        int selected = selectedBoxType == boxtype_none || (selectedBoxType == boxtype_hit && (selectedBoxIdx == i || selectedBoxIdx < 0));
        int opac = 127 * selected + 64 * !selected;
        opac += 100 * (selectedBoxIdx == i && selectedBoxType == boxtype_hit);

        DrawRectangle(
            ((hbr.x + es->position.x) * fighterDrawScale - camera.x) * UNIT_TO_PIX,
            ((hbr.y + es->position.y) * fighterDrawScale - camera.y) * UNIT_TO_PIX,
            hbr.w * UNIT_TO_PIX * fighterDrawScale,
            hbr.h * UNIT_TO_PIX * fighterDrawScale,
            (Color) {255, 0, 0, opac}
        );
    }

    for(int i = 0; i < sb_count(e->subEntities); i++) {
        Entity_DrawHitboxes(e->subEntities[i], camera);
    }
}

void Entity_DrawBlockboxes(Entity* e, RectI camera) {
    EntityState* es = &cb_last(e->history);
    Action* a = es->currentAction;
    printf("Draw boxes\n");
    for(int i = 0; i < sb_count(a->blockboxes); i++) {
        if(a->blockboxes[i]->active != HB_ACTIVE) {
            continue;
        }

        RectI hbr = a->blockboxes[i]->rect;
        // Since we can't *Draw* with negative width, we need to do this
        if (!Entity_FacingRight(e))
            hbr = Rect_Flip_Draw(hbr);

        int selected = selectedBoxType == boxtype_none || (selectedBoxType == boxtype_block && (selectedBoxIdx == i || selectedBoxIdx < 0));
        int opac = 127 * selected + 64 * !selected;
        opac += 100 * (selectedBoxIdx == i && selectedBoxType == boxtype_block);
        DrawRectangle(
            ((hbr.x + es->position.x) * fighterDrawScale - camera.x) * UNIT_TO_PIX,
            ((hbr.y + es->position.y) * fighterDrawScale - camera.y) * UNIT_TO_PIX,
            hbr.w * UNIT_TO_PIX * fighterDrawScale,
            hbr.h * UNIT_TO_PIX * fighterDrawScale,
            (Color) {255, 255, 0, opac}
        );
    }

    for(int i = 0; i < sb_count(e->subEntities); i++) {
        Entity_DrawBlockboxes(e->subEntities[i], camera);
    }
}


Hitbox** Entity_GetHitboxes(Entity* e) {
    EntityState* history = e->history;
    EntityState state = cb_last(history);
    Action* action = state.currentAction;
    Hitbox** hitboxes = action->hitboxes;
    return hitboxes;
}

Hurtbox** Entity_GetHurtboxes(Entity* e) {
    return cb_last(e->history).currentAction->hurtboxes;
}

Entity** Entity_GetRecursive(Entity* e, Entity** entities) {
    sb_push(entities, e);
    for(int i = 0; i < sb_count(e->subEntities); i++) {
        entities = Entity_GetRecursive(e->subEntities[i], entities);
    }
    return entities;
}

Action* Entity_GetAction(Entity* e) {
    EntityState es = cb_last(e->history);
    return es.currentAction;
}

RectI Entity_GlobalizeRect(Entity* e, RectI r) {
        RectI hbr = Entity_FacingRight(e) ? r : Rect_Flip(r);
        hbr.pos = Vec2_Add(hbr.pos, cb_last(e->history).position);
        return hbr;
}

Action* Entity_ShouldHit(Entity* attacker, Entity* defender) {
    Hitbox** hitboxes = Entity_GetHitboxes(attacker);
    Hurtbox** hurtboxes = Entity_GetHurtboxes(defender);

    for(int hit = 0; hit < sb_count(hitboxes); hit++) {
        for(int hurt = 0; hurt < sb_count(hurtboxes); hurt++) {
            if (hitboxes[hit]->active != HB_ACTIVE)
                continue;
            
            RectI r1 = Entity_GlobalizeRect(attacker, hitboxes[hit]->rect);
            RectI r2 = Entity_GlobalizeRect(defender, hurtboxes[hurt]->rect);

             if (Rect_Overlap(r1, r2)) {
                return Entity_GetAction(attacker);
             }
        }
    }

    return NULL;
}


void Entity_Damage(Entity* attacker, Entity* defender, Action* a) {
    EntityState* es = &cb_last(defender->history);

    // No chip damage yet
    if(strcmp(es->currentAction->name, "Block")) {
        es->currentHealth = max(es->currentHealth - a->damage, 0);
    
        if (es->currentHealth <= 0)
            Entity_Die(defender);

        if (a->cb_on_Damage.function) {
            invoke(a->cb_on_Damage);
        }
    }


    // attacker->on_collide();
}

void Entity_Die(Entity* e) {
    cb_last(e->history).valid = false;

    // Queue to free after MAX_REWIND frames?

}

void Entity_Destroy(Entity* e) {

    for (int i = 0; i < cb_count(e->history); i++) {
        free(&e->history[i]);
    }
    cb_free(e->history);

    for (int i = 0; i < sb_count(e->subEntities); i++) {
        Entity_Destroy(e->subEntities[i]);
    }
}