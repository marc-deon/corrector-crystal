#include "Animation.h"
#include "stretchy_buffer.h"
#include "CC_Consts.h"
#include <raylib.h>

int Animation_FindIndexByName(Animation** a, int size, char* name){
    for(int i = 0; i < size; i++){
        int result =  TextIsEqual(a[i]->name, name);
        if(result)
            return i;
    }
    return -1;
}


void Animation_SetLink(Animation* a, Animation* link){
    a->linksTo = link;
}

Animation* Animation_Create(Color colors[256], char* name, char* filename, uint frameCount, uint frameWait, RectI cropRect, int loopStart){

    // Allocate
    Animation* anim = malloc(sizeof(Animation));

    anim->name = name;
    anim->currentFrame = 0;
    anim->frameCount = frameCount;
    anim->frameWait = frameWait;
    anim->loopStart = loopStart;

    // Filename
    Texture imagesurface = LoadTexture(filename);
    // SDL_SetPaletteColors(imagesurface->format->palette, colors, 0, 255);

    // if(imagesurface == 0){
    //     printf("Texture %s not loaded!\n", filename);
    //     return anim;
    // }
    // anim->texture = SDL_CreateTextureFromSurface(ren, imagesurface);
    anim->texture = imagesurface;
    // SDL_FreeSurface(imagesurface);
    // SDL_Texture* txt = anim->texture;
    

    // spriteClips
    anim->spriteClips = 0;
    //
    for(uint i = 0; i < frameCount; i++){
        RectI* rect = malloc(sizeof(RectI));
        rect->pos.x = i*cropRect.size.x;
        rect->pos.y = cropRect.pos.y;
        rect->size.x = cropRect.size.x;
        rect->size.y = cropRect.size.y;
        sb_push(anim->spriteClips, rect);
    }
    

    // Link to self by default. We can explicitly set a link later.
    anim->linksTo = anim;

    return anim;
}

void Animation_Free(Animation* anim){
    // SDL_DestroyTexture(anim->texture);
    free(anim);
}