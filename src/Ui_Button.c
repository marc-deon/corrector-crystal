#include "Ui_Button.h"
#include "stretchy_buffer.h"
// #include <string.h>
#include <raylib.h>
#include "Ui_Types.h"


Font mmbuttfont;

Texture _button_base_texture_normal;
Texture _button_base_texture_p1hover;
Texture _button_base_texture_p2hover;
Texture _button_base_texture_bothhover;
Texture _button_base_texture_pressed;

UiButton* Button_New(char* text, int fontSize){
    mmbuttfont = GetFontDefault();

    _button_base_texture_normal = LoadTexture("Graphics/Ui/asabutton1.png");

    _button_base_texture_p1hover = LoadTexture("Graphics/Ui/asabutton2.png");
    _button_base_texture_p2hover = LoadTexture("Graphics/Ui/asabutton2.png");
    _button_base_texture_bothhover = LoadTexture("Graphics/Ui/asabutton2.png");

    _button_base_texture_pressed = LoadTexture("Graphics/Ui/asabutton3.png");

    UiButton* butt = malloc(sizeof(UiButton));
    butt->id = uiidcount++;
    butt->hcentered = 0;
    butt->vcentered = 0;
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

    butt->fontSize = fontSize;

    butt->string_text = (char*)malloc(TextLength(text) * sizeof(char) + 5);
    TextCopy(butt->string_text, text);
    butt->type = UI_TYPE_BUTTON;
    return butt;
}

void Button_Draw(UiMenu* menu, UiButton* butt, Vec2I offset){
    RectI buttrect;
    Texture button_texture = butt->texture_normal;

    if(menu->p1focused == mqui_as_element(butt) && menu->p2focused == mqui_as_element(butt)){
        button_texture = butt->texture_bothhover;
    }

    else if(menu->p1focused == mqui_as_element (butt)){
        button_texture = butt->texture_p1hover;
    }

    else if(menu->p2focused == mqui_as_element(butt)){
        button_texture = butt->texture_p2hover;
    }


    buttrect = (RectI) {butt->position.x + offset.x, butt->position.y + offset.y, button_texture.width, button_texture.height};
    // printf("but %d at %d %d\n", butt->id, buttrect.x, buttrect.y);

    // TODO: If I understand right, this assumes a single value for kerning spacing? Probably bad for some fonts
    Vector2 v = MeasureTextEx(GetFontDefault(), butt->string_text, butt->fontSize, 2);
    int textx = butt->position.x + offset.x + buttrect.w/2 - v.x/2;
    int texty = butt->position.y + offset.y + buttrect.h/2 - v.y/2;

    DrawTexture(button_texture, buttrect.x, buttrect.y, WHITE);
    DrawText(butt->string_text, textx, texty, butt->fontSize, WHITE);
}

void Button_Free(UiButton* b){
    free(b->string_text);
    free(b);
}

Vec2I Button_Size(UiButton* butt){
    return (Vec2I) {butt->texture_normal.width, butt->texture_normal.height};
}