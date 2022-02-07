#ifndef ANIMATION
#define ANIMATION
#include "Animation.h"
#include "stretchy_buffer.h"
#include "CC_Consts.h"

int Animation_FindIndexByName(Animation** a, int size, char* name){
    for(int i = 0; i < size; i++){
        int result = strcmp(a[i]->name, name);
        if(result == 0)
            return i;
    }
    return -1;
}


void Animation_SetLink(Animation* a, Animation* link){
    a->linksTo = link;
}

Animation* Animation_Create(SDL_Color colors[256], char* name, char* filename, uint frameCount, uint frameWait, SDL_Rect cropRect, SDL_Renderer* ren, int loopStart){

    // Allocate
    Animation* anim = malloc(sizeof(Animation));

    anim->name = name;
    anim->currentFrame = 0;
    anim->frameCount = frameCount;
    anim->frameWait = frameWait;
    anim->loopStart = loopStart;

    // Filename
    SDL_Surface* imagesurface = IMG_Load(filename);
    SDL_SetPaletteColors(imagesurface->format->palette, colors, 0, 255);

    if(imagesurface == 0){
        printf("Image %s not loaded!\n", filename);
        return anim;
    }
    anim->texture = SDL_CreateTextureFromSurface(ren, imagesurface);
    SDL_FreeSurface(imagesurface);
    SDL_Texture* txt = anim->texture;
    

    // spriteClips
    anim->spriteClips = NULL;
    //
    for(uint i = 0; i < frameCount; i++){
        SDL_Rect* rect = malloc(sizeof(SDL_Rect));
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

void Animation_Free(Animation* anim){
    SDL_DestroyTexture(anim->texture);
    free(anim);
}

#endif /* ANIMATION */
