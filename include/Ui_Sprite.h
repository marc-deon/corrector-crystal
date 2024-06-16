#ifndef UI_SPRITE
#define UI_SPRITE
#include "Vec2I.h"
#include "Ui_Types.h"
#include <raylib.h>

typedef struct ui_sprite {
    ui_preamble

    Texture texture;

} UiSprite;

UiSprite* Sprite_New(UiMenu* menu, char* path);
void Sprite_Draw(UiMenu* menu, UiSprite* butt, Vec2I offset);
void Sprite_Free(UiSprite* sprite);


#endif /* UI_SPRITE */
