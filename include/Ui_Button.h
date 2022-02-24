#ifndef UI_BUTTON
#define UI_BUTTON
#include "Ui_Base.h"
#include "Ui_Menu.h"
#include "Vec2I.h"
#include <raylib.h>

typedef struct ui_button {
    ui_preamble
    CallbackFunction on_confirm;
    CallbackFunction on_hold_left;
    CallbackFunction on_hold_right;

    Texture texture_normal;
    
    Texture texture_p1hover;

    Texture texture_p2hover;

    Texture texture_bothhover;

    Texture texture_pressed;

    Texture texture_text;
    char* string_text;

} UiButton;

UiButton* Button_New(char* text, int fontSize);
void Button_Draw(UiMenu* menu, UiButton* butt);
void Button_Free(UiButton*);
Vec2I Button_Size(UiButton* butt);

#endif /* UI_BUTTON */
