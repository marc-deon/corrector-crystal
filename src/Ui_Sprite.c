#include "Ui_Sprite.h"
#include "stretchy_buffer.h"

UiSprite* Sprite_New(UiMenu* menu, char* path){

    UiSprite* sprite = malloc(sizeof(UiSprite));
    sprite->type = UI_TYPE_SPRITE;

    sprite->hcentered = 0;
    sprite->up = NULL;
    sprite->down = NULL;
    sprite->left = NULL;
    sprite->right = NULL;
    sprite->position = (Vec2I){0,0};

    sprite->texture = LoadTexture(path);

    sb_push(menu->elements, (UiElement*)sprite);

    return sprite;
}

void Sprite_Draw(UiMenu* menu, UiSprite* sprite){
    DrawTexture(sprite->texture, sprite->position.x, sprite->position.y, WHITE);
    printf("Drawing texture at %d %d\n", sprite->position.x, sprite->position.y);
}

void Sprite_Free(UiSprite* sprite){

}