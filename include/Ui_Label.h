#ifndef UI_LABEL
#define UI_LABEL
#include "Ui_Base.h"
#include "Vector2.h"
#include "SDL2/SDL.h"
#include "Ui_Menu.h" 

typedef struct ui_label {
    ui_preamble

    SDL_Texture* texture_text;
    SDL_Surface* surface_text;
    char* string_text;
} UiLabel;

UiLabel* Label_New(SDL_Renderer* gRenderer, UiMenu* menu, char* text);
void Label_Draw(SDL_Renderer* gRenderer, UiMenu* menu, UiLabel* label);
void Label_Free(UiLabel* l);
Vector2 Label_Size(SDL_Renderer* ren, UiLabel* butt);

#endif /* UI_LABEL */
