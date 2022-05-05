#include "Ui_Base.h"
#include "Ui_Types.h"
#include "stretchy_buffer.h"
#include "CC_Audio.h"
#include <limits.h>
#include <stdio.h>
#include <assert.h>

Vec2I Menu_Size(UiMenu* menu){
    return menu->size;
}

// centerInBorders only centers horizontally if baketype vertical, or vertically if baketype is horizontal
void Menu_Bake(UiMenu* menu, int centerInBorders){

    int count = sb_count(menu->elements);
    int nonCenteredWidth = 0;
    int nonCenteredHeight = 0;
    int borderCenter;

    int space = 0;
    int sum = 0;

    switch(menu->bakeType) {
        // With a given total size, we need to calculate the spacing.
        case BAKE_TYPE_SET_Y_SIZE: {

            for(int i = 0; i < count; i++){
                UiElement* element = menu->elements[i];

                if(element->type == UI_TYPE_MENU){

                    Menu_Bake(element, false);
                }

                sum += ui_h(element);
                assert(element->hcentered == 0 || element->hcentered == 1);
                nonCenteredWidth = max(nonCenteredWidth, (!element->hcentered) * ui_w(element));
            }
            
            if (count == 1) {
                space = 0;
                }
            else {
                space = ((ui_h(menu) - menu->margin.b - menu->margin.t) - sum) / (count-1);
            }
            borderCenter = (ui_w(menu) + menu->margin.l - menu->margin.r)/2;
            break;
        }

        // With a given total size, we need to calculate the spacing.
        case BAKE_TYPE_SET_X_SIZE: {

            for(int i = 0; i < count; i++){
                UiElement* element = menu->elements[i];

                if(element->type == UI_TYPE_MENU){
                    Menu_Bake(element, false);
                }

                sum += ui_w(element);
                assert(element->vcentered == 0 || element->vcentered == 1);
                nonCenteredHeight = max(nonCenteredHeight, (!element->vcentered) * ui_h(element));
            }

            if (count == 1) {
                space = 0;
            }

            else {
                space = ((ui_w(menu) - menu->margin.r - menu->margin.l) - sum) / (count-1);
            }
            borderCenter = (ui_h(menu) + menu->margin.t - menu->margin.b)/2;
            break;
        }

        case BAKE_TYPE_SET_Y_SPACING: {
            // We'll use the max element width as width, for now
            int maxWidth = 0;
            int heightSum = (count - 1) * space;

            for (size_t i = 0; i < count; i++){
                UiElement* element = menu->elements[i];

                if(element->type == UI_TYPE_MENU){
                    Menu_Bake(element, false);
                }

                else{
                    maxWidth = max(maxWidth, ui_w(element));
                    heightSum += ui_h(element);
                }
            }
            menu->size.x = maxWidth;
            menu->size.y = heightSum;
            break;
        }

        case BAKE_TYPE_SET_X_SPACING: {
            // We'll use the max element height as height, for now
            int maxHeight = 0;
            int widthSum = (count - 1) * space;

            for (size_t i = 0; i < count; i++){
                UiElement* element = menu->elements[i];

                if(element->type == UI_TYPE_MENU){
                    Menu_Bake(element, false);
                }
                
                else{
                    maxHeight = max(maxHeight, ui_h(element));
                    widthSum += ui_w(element);
                }
            }
            menu->size.y = maxHeight;
            menu->size.x = widthSum;
            break;
        }

    }

    sum = 0;

    switch (menu->bakeType) {
        case BAKE_TYPE_SET_Y_SIZE:
        case BAKE_TYPE_SET_Y_SPACING: {
            for(int i = 0; i < count; i++){

                UiElement* element = menu->elements[i];

                // Offset applied to center this element along all other sibling elements
                int xIfElementCentered = nonCenteredWidth/2 - ui_w(element)/2;
                // Offset applied to center this element with respect to the borders
                int xIfCenteredInBorders = borderCenter - nonCenteredWidth/2;

                // Position relative to parent
                element->position.x = element->hcentered*xIfElementCentered + centerInBorders * xIfCenteredInBorders;
                element->position.y = sum;
                sum += ui_h(element) + space;
            }
            break; 
        }
            
        case BAKE_TYPE_SET_X_SIZE:
        case BAKE_TYPE_SET_X_SPACING:{
            for(int i = 0; i < count; i++){
                UiElement* element = menu->elements[i];

                // Offset applied to center this element along all other sibling elements
                int yIfElementCentered = nonCenteredHeight/2 - ui_h(element)/2;
                // Offset applied to center this element with respect to the borders
                int yIfCenteredInBorders = borderCenter - nonCenteredHeight/2;

                // Position relative to parent
                assert(element->vcentered == 0 || element->vcentered == 1);
                element->position.y = element->vcentered*yIfElementCentered + centerInBorders * yIfCenteredInBorders;
                element->position.x = sum;
                sum += ui_w(element) + space;
            }
            break;
        }
            
        default:
            exit(-1);
    }

    int minx = INT_MAX;
    int miny = INT_MAX;
    int maxx = INT_MIN;
    int maxy = INT_MIN;
    for (int i = 0; i < sb_count(menu->elements); i++){
        minx = min(minx, menu->elements[i]->position.x);
        miny = min(miny, menu->elements[i]->position.y);

        maxx = max(maxx, menu->elements[i]->position.x + ui_w(menu->elements[i]));
        maxy = max(maxy, menu->elements[i]->position.y + ui_h(menu->elements[i]));
    }
    menu->size = (Vec2I) {maxx - minx, maxy - miny};
}

bool Menu_Input(UiMenu* menu, Stick* p1stick, Stick* p2stick){
    // TODO(#29): on_hold left/right

    if (menu->p1focused){

        if (Stick_IsButtonJustDown(p1stick, STICK_UP) && menu->p1focused->up){
            CC_Audio_Play_SFX(kirins[kiringtone_current]);
            kiringtone_current = (kiringtone_current+1) % kiringtone_files_len;
            menu->p1focused = menu->p1focused->up;
        }
        else if (Stick_IsButtonJustDown(p1stick, STICK_DOWN) && menu->p1focused->down){
            CC_Audio_Play_SFX(kirins[kiringtone_current]);
            kiringtone_current = (kiringtone_current+1) % kiringtone_files_len;
            menu->p1focused = menu->p1focused->down;
        }
        else if (Stick_IsButtonJustDown(p1stick, STICK_LEFT) && menu->p1focused->left){
            CC_Audio_Play_SFX(kirins[kiringtone_current]);
            kiringtone_current = (kiringtone_current+1) % kiringtone_files_len;
            menu->p1focused = menu->p1focused->left;
        }
        else if (Stick_IsButtonJustDown(p1stick, STICK_RIGHT) && menu->p1focused->right){
            CC_Audio_Play_SFX(kirins[kiringtone_current]);
            kiringtone_current = (kiringtone_current+1) % kiringtone_files_len;
            menu->p1focused = menu->p1focused->right;
        }
        else if (Stick_IsButtonJustDown(p1stick, STICK_START) && menu->p1focused->type == UI_TYPE_BUTTON && mqui_as_button(menu->p1focused)->on_confirm.function){
            CC_Audio_Play_SFX(buttyes);
            invokep(mqui_as_button(menu->p1focused)->on_confirm, 0);
        }
        else if (Stick_IsButtonJustDown(p1stick, STICK_SELECT) && menu->on_cancel.function){
            CC_Audio_Play_SFX(buttno);
            invokep(menu->on_cancel, 0);
            return false;
        }
    }

    if(menu->p2focused){
        if (Stick_IsButtonJustDown(p2stick, STICK_UP) && menu->p2focused->up){
            CC_Audio_Play_SFX(kirins[kiringtone_current]);
            kiringtone_current = (kiringtone_current+1) % kiringtone_files_len;
            menu->p2focused = menu->p2focused->up;
        }
        else if (Stick_IsButtonJustDown(p2stick, STICK_DOWN) && menu->p2focused->down){
            CC_Audio_Play_SFX(kirins[kiringtone_current]);
            kiringtone_current = (kiringtone_current+1) % kiringtone_files_len;
            menu->p2focused = menu->p2focused->down;
        }
        else if (Stick_IsButtonJustDown(p2stick, STICK_LEFT) && menu->p2focused->left){
            CC_Audio_Play_SFX(kirins[kiringtone_current]);
            kiringtone_current = (kiringtone_current+1) % kiringtone_files_len;
            menu->p2focused = menu->p2focused->left;
        }
        else if (Stick_IsButtonJustDown(p2stick, STICK_RIGHT) && menu->p2focused->right){
            CC_Audio_Play_SFX(kirins[kiringtone_current]);
            kiringtone_current = (kiringtone_current+1) % kiringtone_files_len;
            menu->p2focused = menu->p2focused->right;
        }
        else if (Stick_IsButtonJustDown(p2stick, STICK_START) && menu->p2focused->type == UI_TYPE_BUTTON && mqui_as_button(menu->p2focused)->on_confirm.function){
            CC_Audio_Play_SFX(buttyes);
            invokep(mqui_as_button(menu->p2focused)->on_confirm, 1);
        }
        else if (Stick_IsButtonJustDown(p2stick, STICK_SELECT) && menu->on_cancel.function){
            CC_Audio_Play_SFX(buttno);
            invokep(menu->on_cancel, 1);
            return false;
        }
    }

    return true;
}

void Menu_Draw(UiMenu* menu, Vec2I offset){

    if(!menu)
        return;

    RectI rect;
    rect = (RectI) {0, 0, menu->background_texture.width, menu->background_texture.height};
    DrawTexture(menu->background_texture, menu->position.x, menu->position.y, WHITE);

    offset = Vec2_Add(menu->position, offset);
    for(int i = 0; i < sb_count(menu->elements); i++){
        UiElement* element = menu->elements[i];

        // TODO: Add a cascading offset to all of these
        switch(menu->elements[i]->type){
            case UI_TYPE_BUTTON:
                Button_Draw(menu, mqui_as_button(element), offset);
                break;
            case UI_TYPE_LABEL:
                Label_Draw(menu, mqui_as_label(element), offset);
                break;
            case UI_TYPE_SPRITE:
                Sprite_Draw(menu, mqui_as_sprite(element), offset);
                break;

            case UI_TYPE_MATCH:
                break;

            case UI_TYPE_MENU:
                Menu_Draw(mqui_as_menu(element), offset);
                break;

            case UI_TYPE_LIST:
                break;
            
            case UI_TYPE_PREVIEW:
                Preview_Draw(mqui_as_preview(element), offset);
                break;
            
            default:
                exit(2);
        }
    }
}

void Menu_Free(UiMenu* menu){
    sb_free(menu->elements);
}

UiMenu* Menu_New(int baketype){
    UiMenu* m = malloc(sizeof(UiMenu));
    m->id = uiidcount++;
    m->position = (Vec2I) {0,0};
    m->size = (Vec2I) {-1,-1};
    m->margin = (Vec4I) {0,0,0,0};
    m->bakeType = baketype;
    m->on_cancel.function = NULL;
    m->p1focused = 0;
    m->p2focused = 0;
    m->elements = NULL;
    m->type = UI_TYPE_MENU;
    m->hcentered = 0;
    m->vcentered = 0;
    m->spacing = 0;
    return m;
}