#include "Ui_Label.h"
#include "stretchy_buffer.h"
#include <string.h>
#include <raylib.h>
#include <stdio.h>

Font mmlabelfont;
const Color mmlWHITE = (Color) {255, 255, 255, 255};

UiLabel* Label_New(UiMenu* menu, char* text) {
    mmlabelfont = LoadFont("Graphics/Fonts/Recursive-Bold.ttf");
    
    UiLabel* label = malloc(sizeof(UiLabel));
    label->id = uiidcount++;
    label->hcentered = 1;
    label->up = label->down = label->left = label->right = NULL;

    Texture tempsurf;

    label->string_text = (char*)malloc(strlen(text) * sizeof(char) + 1);
    TextCopy(label->string_text, text);
    label->type = UI_TYPE_LABEL;
    sb_push(menu->elements, (UiElement*)label);

    return label;
}

void Label_Draw(UiMenu* menu, UiLabel* label, Vec2I offset) {
    
    
    DrawText(label->string_text, label->position.x + offset.x, label->position.y + offset.y, 20, WHITE);
}

void Label_Free(UiLabel* l) {
    free(l->string_text);
    free(l);
}

Vec2I Label_Size(UiLabel* label) {
    Vector2 v = MeasureTextEx(GetFontDefault(), label->string_text, 20, 0);
    return (Vec2I) {v.x, v.y};
    
}