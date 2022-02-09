#include "Ui_Button.h"
#include "stretchy_buffer.h"
// #include <string.h>
#include <raylib.h>
#include "Ui_Types.h"


// TODO: Variable font size
#define FONTSIZE 20

Font mmbuttfont;

Texture _button_base_texture_normal;
Texture _button_base_texture_p1hover;
Texture _button_base_texture_p2hover;
Texture _button_base_texture_bothhover;
Texture _button_base_texture_pressed;

UiButton* Button_New(UiMenu* menu, char* text, int fontSize){
    mmbuttfont = GetFontDefault();

    _button_base_texture_normal = LoadTexture("Graphics/Ui/button1.png");
    _button_base_texture_p1hover = LoadTexture("Graphics/Ui/button2.png");
    _button_base_texture_p2hover = LoadTexture("Graphics/Ui/button2.png");
    _button_base_texture_bothhover = LoadTexture("Graphics/Ui/button2.png");

    _button_base_texture_pressed = LoadTexture("Graphics/Ui/button3.png");

    UiButton* butt = malloc(sizeof(UiButton));
    butt->hcentered = 0;
    butt->up    = NULL;
    butt->down  = NULL;
    butt->left  = NULL;
    butt->right = NULL;
    butt->position = (Vec2I){0,0};
    butt->on_confirm    = (CallbackFunction){};
    butt->on_hold_left  = (CallbackFunction){};
    butt->on_hold_right = (CallbackFunction){};

    butt->texture_normal = _button_base_texture_normal;
    butt->texture_p1hover = _button_base_texture_p1hover;
    butt->texture_p2hover = _button_base_texture_p2hover;
    butt->texture_bothhover = _button_base_texture_bothhover;
    butt->texture_pressed = _button_base_texture_pressed;

    butt->string_text = (char*)malloc(TextLength(text) * sizeof(char) + 1);
    // butt->texture_text = TTF_RenderText_Blended(mmbuttfont, text, mmb_WHITE);
    // butt->texture_text = SDL_CreateTextureFromSurface(gRenderer, butt->texture_text);
    TextCopy(butt->string_text, text);
    butt->type = UI_TYPE_BUTTON;
    sb_push(menu->elements, (UiElement*)butt);

    return butt;
}

void Button_Draw(UiMenu* menu, UiButton* butt){
    RectI rect, buttrect;
    Texture tex = butt->texture_normal;

    if(menu->p1focused == mqui_as_element(butt) && menu->p2focused == mqui_as_element(butt)){
        tex = butt->texture_bothhover;
    }

    else if(menu->p1focused == mqui_as_element (butt)){
        tex = butt->texture_p1hover;
    }

    else if(menu->p2focused == mqui_as_element(butt)){
        tex = butt->texture_p2hover;
    }


    buttrect = rect = (RectI) {butt->position.x, butt->position.y, tex.width, tex.height};

    // Center words
    Vector2 v = MeasureTextEx(GetFontDefault(), butt->string_text, FONTSIZE, 0);
    rect = (RectI) {butt->position.x, butt->position.y, v.x, v.y};
    rect.x = butt->position.x + buttrect.w/2 - rect.w/2;
    rect.y = butt->position.y + buttrect.h/2 - rect.h/2;

    DrawTexture(tex, butt->position.x, butt->position.y, WHITE);
    DrawText(butt->string_text, rect.x, rect.y, FONTSIZE, WHITE);
}

void Button_Free(UiButton* b){
    free(b->string_text);
    free(b);
}

Vec2I Button_Size(UiButton* butt){
    return (Vec2I) {butt->texture_normal.width, butt->texture_normal.height};
}