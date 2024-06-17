#include "circular_buffer.h"
#include "Entities.h"
#include "stretchy_buffer.h"
#include "circular_buffer.h"
#include "Json_Extension.h"
#include "Fighter.h"

#include <stdio.h>
#include <json-c/json.h>

EntityState** unowned_entities = 0;

void temp_fireball_on_hit(CallbackInfo info) {
    Entity_Die(info.target);
    // Fighter_Damage((Fighter*) ((Entity*)info.target)->fighter), )
    
    printf("doot %s %d\n", ((Entity*)info.target)->name, ((Entity*)info.target)->history->valid);
    printf("Inside invoke %p %c\n", info.target, ((Entity*)info.target)->history->valid ? 'T' : 'F');
}


// TODO: Recursive copy of subentities, if neccesary. Hopefully not neccesary. Those would be complicated projectiles.
// It is, without a doubt, in this function
Entity* Entity_Copy(Entity* old_e) {

    Entity* new_e = malloc(sizeof(Entity));

    memcpy(new_e, old_e, sizeof(Entity));

    new_e->subEntities = NULL;
    for(int i = 0; i < sb_count(old_e->subEntities); i++) {
        sb_push(new_e->subEntities, old_e->subEntities[i]);
    }


    new_e->history = NULL;
    cb_init(new_e->history, MAX_REWIND);
    EntityState es = cb_last(old_e->history);
    cb_push(new_e->history, es);

    EntityState* es2 = &cb_last(new_e->history);
    es2->currentAnimation = Animation_Copy(es.currentAnimation);
    es2->currentAction = Action_Copy(es.currentAction);

    return new_e;
}

// returns a partially-initd Entity with partially-initd State, which should not be modified directly; copy it first.
Entity* Entity_ReadPartial(char* path) {

    struct json_object* parsed_json = json_get_parsed_json(path);
    Entity* e;
    int rect[6];

    char* inherit = json_get_default_string(parsed_json, "inherit", NULL);

    // Entitity proper
    // e = inherit ? Entity_ReadPartial(inherit) : Entity_Create(NULL);
    e = Entity_Create(NULL);
    e->name = json_get_string(parsed_json, "name");
    e->collisionMask = json_get_default_int(parsed_json, "collisionMask", 0b11);
    e->shader.id = 0;

    // Animation
    json_get_int_array(parsed_json, "cropRect", rect);
    Animation* a = Animation_Create(
        json_get_string(parsed_json, "animationName"),
        json_get_string(parsed_json, "filename"),
        json_get_int(parsed_json, "frameCount"),
        json_get_int(parsed_json, "frameWait"),
        (RectI) {rect[0], rect[1], rect[2], rect[3]},
        json_get_default_int(parsed_json, "loopStart", 0)
    );

    // Entity state
    json_get_default_int_array(parsed_json, "velocity", rect, 2);
    EntityState* es = EntityState_Create();
    es->velocity = (Vec2I) {rect[0], rect[1]};
    es->currentHealth = 1;
    es->currentAnimation = a;
    es->currentAction = Action_CreateNull();
    es->currentAction->damage = json_get_int(parsed_json, "damage");
    es->currentAction->name = json_get_string(parsed_json, "name");
    es->currentAction->hitstun = json_get_default_int(parsed_json, "hitstun", 0);
    es->currentAction->hitStop = json_get_default_int(parsed_json, "hitstop", 0);
    // Set defaults for hitbox onFrame, offFrame
    rect[4] = 0; rect[5] = -1;
    json_get_int_array(parsed_json, "hitbox", rect);
    Hitbox* hb = Hitbox_Create((RectI) {rect[0], rect[1], rect[2], rect[3]}, rect[4], rect[5]);
    sb_push(es->currentAction->hitboxes, hb);
    json_get_int_array(parsed_json, "position", rect);
    es->position = (Vec2I) {rect[0], rect[1]};

    cb_push(e->history, *es);

    return e;
}

void Entity_Initialize_Partial(Entity* partial, Entity* owner) {
    EntityState* es = &(cb_last(partial->history));
    es->valid = true;
    partial->owner = owner;
    partial->fighter = owner->fighter;
    partial->shader.id = 0;
    // partial->palette = owner->palette;
    partial->paletteNumber = owner->paletteNumber;
    partial->paletteTexture = owner->paletteTexture;

    es->facingRight = cb_last(owner->history).facingRight;
    es->velocity.x *= es->facingRight ? 1 : -1;
    es->position.x *= es->facingRight ? 1 : -1;

    es->position = Vec2_Add(es->position, cb_last(owner->history).position);
    
    es->currentAction->cb_on_Damage.function = temp_fireball_on_hit;
    es->currentAction->cb_on_Damage.info.target = partial;

    sb_push(owner->subEntities, partial);
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
