#ifndef ANIMATION_H
#define ANIMATION_H
// #include <SDL2/SDL.h>
#include "qtypes.h"
#include <raylib.h>
#include "RectI.h"

typedef struct animation{
    char* name;
    Texture* texture;
    uint currentFrame;
    uint frameCount;
    uint frameWait; // 1/frameRate. The number of frames to wait before incrementing the displayed frame. "Animated on X's".
    uint loopStart;
    RectI** spriteClips; // Stretchy buffer; RectIs for each sprite frame
    struct animation* linksTo;

} Animation;

int Animation_FindIndexByName(Animation** a, int size, char* name);
void Animation_SetLink(Animation* a, Animation* link);
Animation* Animation_Create(Color colors[256], char* name, char* filename, uint frameCount, uint frameWait, RectI cropRect, int loopStart);
void Animation_Free(Animation* anim);

#endif /* ANIMATION */
