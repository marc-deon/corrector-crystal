#include "Ui_Sprite.h"
#include "stretchy_buffer.h"

UiSprite* Sprite_New(SDL_Renderer* gRenderer, UiMenu* menu, char* path){

    UiSprite* sprite = malloc(sizeof(UiSprite));
    sprite->type = UI_TYPE_SPRITE;

    sprite->hcentered = 0;
    sprite->up = NULL;
    sprite->down = NULL;
    sprite->left = NULL;
    sprite->right = NULL;
    sprite->position = (Vector2){0,0};

    sprite->surface = IMG_Load(path);
    sprite->texture = SDL_CreateTextureFromSurface(gRenderer, sprite->surface);


    sb_push(menu->elements, (UiElement*)sprite);

    return sprite;
}

void Sprite_Draw(SDL_Renderer* gRenderer, UiMenu* menu, UiSprite* sprite){
    SDL_Rect rect = (SDL_Rect){sprite->position.x, sprite->position.y, sprite->surface->w, sprite->surface->h};
    SDL_RenderCopy(gRenderer, sprite->texture, NULL, &rect);
}

void Sprite_Free(UiSprite* sprite){

}