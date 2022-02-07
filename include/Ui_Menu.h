#ifndef UI_MENU
#define UI_MENU

#include "Ui_Base.h"

typedef struct ui_menu {
    ui_preamble
    CallbackFunction on_cancel;
    struct ui_element* p1focused;
    struct ui_element* p2focused;
    SDL_Texture* background_texture;
    SDL_Surface* background_surface;
    struct ui_element** elements; // stretchy
} UiMenu;

UiMenu* Ui_GetTopFocus();
UiMenu* Ui_PopFocus();
UiMenu* Ui_PushFocus(UiMenu* focus);

void Ui_CloseTop();
void Menu_Bake(SDL_Renderer* ren, UiMenu* Menu, Vector4 borders, Vector4 border_offset, int baketype, int centerAll);
void Menu_Draw(SDL_Renderer* ren, UiMenu* Menu);
bool Menu_Input(UiMenu* menu, Stick* p1stick, Stick* p2stick);
void Menu_Free(UiMenu* menu);

#endif /* UI_MENU */
