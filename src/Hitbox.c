#include <stdlib.h>
#include <stdio.h>
#include "Hitbox.h"


Hitbox NULLHIT;
Hurtbox NULLHURT;

Shovebox NULLSHOVE = {.rect = {0,0,0,0}};

Hitbox* Hitbox_Create(RectI rect, ushort activeOnFrame, ushort offOnFrame) {
    Hitbox* hb = (Hitbox*)malloc(sizeof(Hitbox));
    hb->rect = rect;
    hb->activeOnFrame = activeOnFrame;
    hb->offOnFrame = offOnFrame;
    
    hb->active = HB_INACTIVE;
    hb->currentFrame = 0;
}

Hurtbox* Hurtbox_Create(RectI rect) {
    Hurtbox* hb = (Hurtbox*)malloc(sizeof(Hitbox));
    hb->rect = rect;
    return hb;
}
Shovebox Shovebox_Create(RectI rect) {
    Shovebox sb;
    sb.rect = rect;
    return sb;
}

void Hitbox_UpdateTimer(Hitbox* hb) {

    // If HB_INACTIVE, then the hitbox hasn't gone live yet. If HB_DISABLED, then it shouldn't go live.
    if(hb->currentFrame == hb->activeOnFrame && hb->active == HB_INACTIVE) {
        hb->active = HB_ACTIVE;
        // printf("Activating hitbox frame %d\n", hb->activeOnFrame);
    }

    if(hb->currentFrame == hb->offOnFrame) {
        hb->active = HB_INACTIVE;
    }

    hb->currentFrame ++;
}