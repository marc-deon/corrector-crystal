#include "Animation.h"
#include "stretchy_buffer.h"
#include "CC_Consts.h"
#include <raylib.h>
#include <stdio.h>

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

    // TODO: Palette stuff needs to be done in a shader instead

    // Load image
    // Image img = LoadImage(filename);
    
    // // Load images's base palette
    // int colorCount;
    // Color* baseColors = LoadImagePalette(img, 256, &colorCount);
    // // Color* baseColors = LoadImageColors(img);

    // // For each base color, replace with new color
    // // NOTE: This may require some shenanigans... or we use shaders later. Probably shaders.
    // for(int i = 0; i < colorCount; i++){
    //     ImageColorReplace(&img, baseColors[i], colors[i]);
    // }

    // // Convert image to texture
    // Texture imagesurface = LoadTextureFromImage(img);

    // // Unload image
    // UnloadImage(img);

    printf("Loading anim from sheet %s\n", filename);
    Texture t = LoadTexture(filename);
    
    anim->texture = t;
    


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
    free(anim);
}