#ifndef HITBOX_H
#define HITBOX_H
#include <stdbool.h>
#include "Rectangle.h"


#define HB_ACTIVE   1
#define HB_INACTIVE 0
#define HB_DISABLED -1

typedef unsigned short ushort;

typedef struct hitbox {
    Rectangle rect;
    
    // 1 when active; 0 when not yet active; -1 when disabled.
    char active; 

    ushort activeOnFrame;
    ushort offOnFrame;
    ushort currentFrame;

} Hitbox;

typedef struct hurtbox{
    Rectangle rect;
} Hurtbox;

typedef struct shovebox{
    Rectangle rect;
} Shovebox;

extern Hitbox NULLHIT;
extern Hurtbox NULLHURT;
extern Shovebox NULLSHOVE;

Hitbox* Hitbox_Create(Rectangle rect, ushort activeOnFrame, ushort offOnFrame);
Hurtbox* Hurtbox_Create(Rectangle rect);
Shovebox Shovebox_Create(Rectangle rect);

Hitbox Hitbox_Flip(Hitbox hb);
Hurtbox Hurtbox_Flip(Hurtbox hb);
Shovebox Shovebox_Flip(Shovebox hb);

void Hitbox_Activate(Hitbox* hb);
void Hitbox_Deactivate(Hitbox* hb);
void Hitbox_UpdateTimer(Hitbox* hb);

#endif