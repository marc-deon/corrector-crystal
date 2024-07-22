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
    boxtype_block,
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

typedef struct hurtbox {
    RectI rect;
} Hurtbox;

typedef struct shovebox {
    RectI rect;
} Shovebox;

typedef Hitbox Blockbox;

extern Hitbox NULLHIT;
extern Hurtbox NULLHURT;
extern Shovebox NULLSHOVE;
extern Blockbox BLOCKBOX;

Hitbox* Hitbox_Create(RectI rect, ushort activeOnFrame, ushort offOnFrame);
Hurtbox* Hurtbox_Create(RectI rect);
Shovebox Shovebox_Create(RectI rect);
Blockbox Blockbox_Create(RectI rect);

void Hitbox_UpdateTimer(Hitbox* hb);

#endif /* HITBOX */
