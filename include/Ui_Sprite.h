#ifndef UI_SPRITE
#define UI_SPRITE
#include "SDL2/SDL_image.h"
#include "Vector2.h"
#include "Ui_Types.h"

typedef struct ui_sprite {
    ui_preamble

    SDL_Surface* surface;
    SDL_Texture* texture;

} UiSprite;

UiSprite* Sprite_New(SDL_Renderer* gRenderer, UiMenu* menu, char* path);
void Sprite_Draw(SDL_Renderer* gRenderer, UiMenu* menu, UiSprite* butt);
void Sprite_Free(UiSprite* sprite);


#endif /* UI_SPRITE */
