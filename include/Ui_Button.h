#ifndef UI_BUTTON
#define UI_BUTTON
#include "Ui_Base.h"
#include "Ui_Menu.h"
#include "SDL2/SDL_image.h"
#include "Vector2.h"

typedef struct ui_button {
    ui_preamble
    CallbackFunction on_confirm;
    CallbackFunction on_hold_left;
    CallbackFunction on_hold_right;

    SDL_Surface* surface_normal;
    SDL_Texture* texture_normal;
    
    SDL_Surface* surface_p1hover;
    SDL_Texture* texture_p1hover;

    SDL_Surface* surface_p2hover;
    SDL_Texture* texture_p2hover;

    SDL_Surface* surface_bothhover;
    SDL_Texture* texture_bothhover;

    SDL_Surface* surface_pressed;
    SDL_Texture* texture_pressed;

    SDL_Texture* texture_text;
    SDL_Surface* surface_text;
    char* string_text;

} UiButton;

UiButton* Button_New(SDL_Renderer* gRenderer, UiMenu* menu, char* text, int fontSize);
void Button_Draw(SDL_Renderer* gRenderer, UiMenu* menu, UiButton* butt);
void Button_Free(UiButton*);
Vector2 Button_Size(SDL_Renderer* ren, UiButton* butt);

#endif /* UI_BUTTON */
