#include "circular_buffer.h"
#include "Entities.h"
#include "stretchy_buffer.h"
#include "circular_buffer.h"

#include <stdio.h>

EntityState** unowned_entities = 0;

void temp_fireball_on_hit(CallbackInfo info) {
    Entity_Die(info.target);
    printf("doot\n");
}

Entity* Entities_CreateAokoFireballA(Entity* owner) {
    printf("Fireball created\n");
    Entity* e = Entity_Create(owner);
    e->name = "Fireball";
    // e->shader = owner->shader;
    e->shader.id = 0;
    e->collisionMask = COL_MASK_PLAYER | COL_MASK_PROJECTILE;

    EntityState* es = EntityState_Create();
    es->facingRight = Entity_FacingRight(owner);
    es->valid = true;
    cb_push(e->history, *es);
    es = &cb_last(e->history);
    es->position = cb_last(owner->history).position;
    es->velocity = (Vec2I) {5, 0};
    if (!es->facingRight)
        es->velocity.x *= -1;
    es->currentAnimation = Animation_Create("Fireballball",
        "Graphics/Fighter/Shoto/ryu_fireball_loop.png",
        2, 4, (RectI) {0,0,639,479}, 0
    );

// "cropRect": [0,0, 639, 479]

    es->currentAction = Action_CreateNull();
    es->currentAction->damage = 1000;
    es->currentAction->cb_on_Damage.function = temp_fireball_on_hit;
    es->currentAction->cb_on_Damage.info.target = e;
    Hitbox* hb = Hitbox_Create((RectI) {0, -500, 639, 479} , 0, -1);
    sb_push(es->currentAction->hitboxes, hb);
    return e;
}

Entity* Entities_CreateAokoFireballB(Entity* owner) {
    Entity* e = Entities_CreateAokoFireballA(owner);
    cb_last(e->history).velocity.x = 10;
    if (!Entity_FacingRight(e))
        cb_last(e->history).velocity.x *= -1;
    return e;
}

Entity* Entities_CreateAokoFireballC(Entity* owner) {
    Entity* e = Entities_CreateAokoFireballA(owner);
    cb_last(e->history).velocity.x = 15;
    if (!Entity_FacingRight(e))
        cb_last(e->history).velocity.x *= -1;
    return e;
}

int Entities_GetEntityFromName(char* name) {
    if (!name)
        return ENTITY_NONE;
    return_str_to_enum(name, ENTITY_AOKO_FIREBALL_A)
    return_str_to_enum(name, ENTITY_AOKO_FIREBALL_B)
    return_str_to_enum(name, ENTITY_AOKO_FIREBALL_C)
    return_str_to_enum(name, ENTITY_AOKO_FIREBALL_EXPLODE)
    return ENTITY_NONE;
}

// The owner and position are not neccesarily both used
Entity* Entities_SpawnType(enum EntityType type, Entity* owner, Vec2I position) {
    Entity* e = NULL;

    if (type)
        printf("Spawning with type %d\n", type);
    else
        printf("Invalid spawn type %d\n", type);

    switch(type) {
        case ENTITY_AOKO_FIREBALL_A:
            e = Entities_CreateAokoFireballA(owner);
            break;
        case ENTITY_AOKO_FIREBALL_B:
            e = Entities_CreateAokoFireballB(owner);
            break;
        case ENTITY_AOKO_FIREBALL_C:
            e = Entities_CreateAokoFireballC(owner);
            break;
        case ENTITY_AOKO_FIREBALL_EXPLODE:
            break;
    }

    return e;
}

