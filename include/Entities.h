#ifndef ENTITIES_H
#define ENTITIES_H

#include "Entity.h"
#include "string.h"

extern float fighterDrawScale;

enum EntityType {
    
    ENTITY_NONE,
    ENTITY_AOKO_FIREBALL_A,
    ENTITY_AOKO_FIREBALL_B,
    ENTITY_AOKO_FIREBALL_C,
    ENTITY_AOKO_FIREBALL_EXPLODE

};

#define return_str_to_enum(s, e) {if (!strcmp(#e, s)) return e;}

int Entities_GetEntityFromName(char* name);
Entity* Entities_SpawnType(enum EntityType type, Entity* owner, Vec2I position);


#endif