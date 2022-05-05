#ifndef HITBOX
#define HITBOX
#include <stdbool.h>
#include "RectI.h"


#define HB_ACTIVE   1
#define HB_INACTIVE 0
#define HB_DISABLED -1

typedef unsigned short ushort;

enum boxtype {
    boxtype_none,
    boxtype_hit,
    boxtype_hurt,
    boxtype_shove,
    boxtype_max
};

typedef struct hitbox {
    RectI rect;
    
    // 1 when active; 0 when not yet active; -1 when disabled.
    char active; 

    ushort activeOnFrame;
    ushort offOnFrame;
    ushort currentFrame;

} Hitbox;

typedef struct hurtbox{
    RectI rect;
} Hurtbox;

typedef struct shovebox{
    RectI rect;
} Shovebox;

extern Hitbox NULLHIT;
extern Hurtbox NULLHURT;
extern Shovebox NULLSHOVE;

Hitbox* Hitbox_Create(RectI rect, ushort activeOnFrame, ushort offOnFrame);
Hurtbox* Hurtbox_Create(RectI rect);
Shovebox Shovebox_Create(RectI rect);

Hitbox Hitbox_Flip(Hitbox hb);
Hurtbox Hurtbox_Flip(Hurtbox hb);
Shovebox Shovebox_Flip(Shovebox hb);

void Hitbox_Activate(Hitbox* hb);
void Hitbox_Deactivate(Hitbox* hb);
void Hitbox_UpdateTimer(Hitbox* hb);

#endif /* HITBOX */
