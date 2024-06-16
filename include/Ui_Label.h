#ifndef UI_LABEL
#define UI_LABEL
#include "Ui_Base.h"
#include "Vec2I.h"
#include "Ui_Menu.h" 

typedef struct ui_label {
    ui_preamble

    char* string_text;
} UiLabel;

UiLabel* Label_New(UiMenu* menu, char* text);
void Label_Draw(UiMenu* menu, UiLabel* label, Vec2I offset);
void Label_Free(UiLabel* l);
Vec2I Label_Size(UiLabel* butt);

#endif /* UI_LABEL */
