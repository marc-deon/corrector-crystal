#include "Animation.h"
#include "stretchy_buffer.h"
#include "CC_Consts.h"
#include <raylib.h>
#include <stdio.h>
#include <string.h>

int Animation_FindIndexByName(Animation** a, int size, char* name) {
    for(int i = 0; i < size; i++) {
        int result =  TextIsEqual(a[i]->name, name);
        if(result)
            return i;
    }
    return -1;
}


void Animation_SetLink(Animation* a, Animation* link) {
    a->linksTo = link;
}

Animation* Animation_Copy(Animation* old_a) {
    Animation* anim = malloc(sizeof(Animation));
    anim->name = malloc(sizeof(char) * strlen(old_a->name));
    strcpy(anim->name, old_a->name);
    anim->currentFrame = 0;
    anim->frameCount = old_a->frameCount;
    anim->frameWait = old_a->frameWait;
    anim->loopStart = old_a->loopStart;
    anim->texture = LoadTextureFromImage(LoadImageFromTexture(old_a->texture));
    
    printf("old id %d new id %d\n", old_a->texture.id, anim->texture.id);

    anim->spriteClips = NULL;
    for(uint i = 0; i < anim->frameCount; i++) {
        RectI* rect = malloc(sizeof(RectI));
        *rect = *(old_a->spriteClips[i]);
        sb_push(anim->spriteClips, rect);
    }

    // Link to self by default. We can explicitly set a link later.
    anim->linksTo = anim;

    return anim;
}

Animation* Animation_Create(char* name, char* filename, uint frameCount, uint frameWait, RectI cropRect, int loopStart) {

    // Allocate
    Animation* anim = malloc(sizeof(Animation));

    anim->name = name;
    anim->currentFrame = 0;
    anim->frameCount = frameCount;
    anim->frameWait = frameWait;
    anim->loopStart = loopStart;
    anim->texture_filename = malloc(sizeof(char) * strlen(filename) + 1);
    strcpy(anim->texture_filename, filename);
    anim->texture = LoadTexture(filename);
    
    // spriteClips
    anim->spriteClips = NULL;
    for(uint i = 0; i < frameCount; i++) {
        RectI* rect = malloc(sizeof(RectI));
        rect->x = i*cropRect.w;
        rect->y = cropRect.y;
        rect->w = cropRect.w;
        rect->h = cropRect.h;
        sb_push(anim->spriteClips, rect);
    }
    

    // Link to self by default. We can explicitly set a link later.
    anim->linksTo = anim;

    return anim;
}

void Animation_Free(Animation* anim) {
    free(anim);
}