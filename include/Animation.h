#ifndef ANIMATION_H
#define ANIMATION_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <List.h>
#include "qtypes.h"

typedef struct animation{
    char* name;
    SDL_Texture* texture;
    uint currentFrame;
    uint frameCount;
    uint frameWait; // 1/frameRate. The number of frames to wait before incrementing the displayed frame. "Animated on X's".
    uint loopStart;
    SDL_Rect** spriteClips; // Stretchy buffer; Rectangles for each sprite frame
    struct animation* linksTo;

} Animation;

int Animation_FindIndexByName(Animation** a, int size, char* name);
void Animation_SetLink(Animation* a, Animation* link);
Animation* Animation_Create(SDL_Color colors[256], char* name, char* filename, uint frameCount, uint frameWait, SDL_Rect cropRect, SDL_Renderer* ren, int loopStart);
void Animation_Free(Animation* anim);

#endif
