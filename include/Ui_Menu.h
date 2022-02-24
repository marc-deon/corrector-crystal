#ifndef UI_MENU
#define UI_MENU

#include "Ui_Base.h"

typedef struct ui_menu {
    ui_preamble
    CallbackFunction on_cancel;
    struct ui_element* p1focused;
    struct ui_element* p2focused;
    Texture background_texture;
    
    enum bake_type bakeType;
    Vec2I size;
    // Internal margin that separates children from edges
    Vec4I margin;
    // Only used when baketype is BAKE_TYPE_SET_SPACING
    int spacing; 

    struct ui_element** elements; // stretchy
} UiMenu;

UiMenu* Ui_GetTopFocus();
UiMenu* Ui_PopFocus();
UiMenu* Ui_PushFocus(UiMenu* focus);

void Ui_CloseTop();
void Menu_Bake(UiMenu* Menu, int centerInBorders);
void Menu_Draw(UiMenu* Menu);
bool Menu_Input(UiMenu* menu, Stick* p1stick, Stick* p2stick);
void Menu_Free(UiMenu* menu);
UiMenu* Menu_New(int baketype);
Vec2I Menu_Size(UiMenu* menu);

#endif /* UI_MENU */
