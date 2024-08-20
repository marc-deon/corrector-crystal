#ifndef ENTITY_H
#define ENTITY_H
#include "Vec2I.h"
#include "Hitbox.h"
#include "CC_Consts.h"
#include "EntityState.h"
#include "Types.h"

#include "Vec2I.h"
#include <stdbool.h>
#include "Animation.h"
#include "Action.h"

typedef struct EntityState {
    bool valid;
    Vec2I position;
    Vec2I velocity;
    bool facingRight;

    Action* currentAction;
    Animation* currentAnimation;

    short currentHealth;

    unsigned char frame;
    unsigned char subframe;

} EntityState;

EntityState* EntityState_Create();

typedef struct entity {
    char* name;
    Color palette[256];
    uint paletteNumber;
    Texture paletteTexture;
    struct entity* owner;
    // Void to avoid circular include
    void* fighter;               // Null unless this entity is the root of the fighter
    struct entity** subEntities; // Stretchy
    EntityState* history;        // Circular
    // CallbackFunction on_collide;
    CallbackFunction on_update;
    int collisionMask;
    Shader shader;

} Entity;

void Entity_Process_Advance(Entity* e);
void Entity_Process_Position(Entity* e);
void Entity_Process_Hitbox(Entity* e);

void Entity_StartAction(Entity* e, Action* a, int setMax);

Entity* Entity_Create(Entity* owner);
void Entity_Move(Entity* es);
bool Entity_FacingRight(Entity* es);
bool Entity_FacingRight(Entity* e);
void Entity_DrawHitboxes(Entity* e, RectI camera);
void Entity_DrawBlockboxes(Entity* e, RectI camera);
void Entity_DrawSprite(Entity* e, RectI camera);
void Entity_ChangeAnimation(Entity* e, Animation* newAnimation);
/* Return a flat depth-first list of entities */
Entity** Entity_GetRecursive(Entity* e, Entity** entities);
/* Return attacker action that hits defender, or null. Not recursive. */
Action* Entity_ShouldHit(Entity* attacker, Entity* defender);
/* Return list of current hitboxes */
Hitbox** Entity_GetHitboxes(Entity* e);
/* Return list of current hurtboxes */
Hurtbox** Entity_GetHurtboxes(Entity* e);
/* Return current action */
Action* Entity_GetAction(Entity* e);

void Entity_Damage(Entity* attacker, Entity* defender, Action* a);
void Entity_Destroy(Entity* e);
void Entity_Die(Entity* e);
void Entity_BeginBlock(Entity* e, int blockstun);

#endif // ENTITY.H