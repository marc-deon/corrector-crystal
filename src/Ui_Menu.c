#include "Ui_Base.h"
#include "Ui_Types.h"
#include "stretchy_buffer.h"
#include "CC_Audio.h"
#include <limits.h>
#include <stdio.h>

Vec2I Menu_Size(UiMenu* menu){
    
    int minx = INT_MAX;
    int maxx = INT_MIN;
    int miny = INT_MAX;
    int maxy = INT_MIN;

    for (size_t i = 0; i < sb_count(menu->elements); i++)
    {
        UiElement* elem = menu->elements[i];
        minx = min(elem->position.x, minx);
        miny = min(elem->position.y, miny);

        maxx = max(elem->position.x + UiElement_Size(elem).x, maxx);
        maxy = max(elem->position.y + UiElement_Size(elem).y, maxy);
    }
    return (Vec2I){maxx-minx, maxy-miny};
}

// When baketype is BAKE_TYPE_SET_SPACING, the b component of borders is used as the spacing
// centerInBorders only centers horizontally if baketype vertical, or vertically if baketype is horizontal
void Menu_Bake(UiMenu* menu, Vec4I borders, Vec4I border_offset, int baketype, int centerInBorders){

    if(baketype == BAKE_TYPE_DEFAULT)
        baketype = menu->bakeType;

    menu->position = (Vec2I) {borders.l, borders.t};
    int count = sb_count(menu->elements);
    int ySpace;
    int nonCenteredWidth = 0;
    int borderCenterX;
    int xSpace;
    int nonCenteredHeight = 0;
    int borderCenterY;
    int ySum = 0;
    int xSum = 0;

    // TODO(#28): Make an actual Menu type, store the borders on that, and draw children relative to it

    switch(baketype){
        // With a given total size, we need to calculate the spacing.
        case BAKE_TYPE_SET_Y_SIZE: {

            for(int i = 0; i < count; i++){
                UiElement* element = menu->elements[i];

                if(element->type == UI_TYPE_MENU){
                    Vec4I subbord = borders;
                    subbord.t += ySum;
                    printf("baking sub with %d %d %d %d\n", borders.l, borders.r, borders.t, borders.b, borders);
                    Menu_Bake(mqui_as_menu(element), subbord, (Vec4I) {.t = ySum}, mqui_as_menu(element)->bakeType, false);
                }

                Vec2I elSize = UiElement_Size(element);
                ySum += elSize.y;
                nonCenteredWidth = max(nonCenteredWidth, (!element->hcentered) * elSize.x);
            }

            ySpace = ((borders.b - borders.t - border_offset.b - border_offset.t) - ySum) / (count-1);
            borderCenterX = (borders.r - borders.l + border_offset.l - border_offset.r)/2;
            break; }

        // With a given total size, we need to calculate the spacing.
        case BAKE_TYPE_SET_X_SIZE:

            for(int i = 0; i < count; i++){
                UiElement* element = menu->elements[i];

                if(element->type == UI_TYPE_MENU){
                    Vec4I subbord = borders;
                    subbord.l += xSum;
                    printf("subbake %d %d\n", subbord.l, xSum);
                    Menu_Bake(mqui_as_menu(element), subbord, (Vec4I) {.l = xSum}, mqui_as_menu(element)->bakeType, false);
                }

                Vec2I elSize = UiElement_Size(element);
                xSum += elSize.x;
                nonCenteredHeight = max(nonCenteredHeight, (!element->vcentered) * elSize.y);
            }

            xSpace = ((borders.r - borders.l - border_offset.r - border_offset.l) - xSum) / (count-1);
            borderCenterY = (borders.r - borders.l + border_offset.l - border_offset.r)/2;
            break;

        case BAKE_TYPE_SET_Y_SPACING: {
            // With a given spacing, we need to calculate the bounding box (borders).
            ySpace = borders.b;


            borders.t = 0;
            borders.b = (count-1) * ySpace + ySum + border_offset.t + border_offset.t;
            borders.l = 0;

            // We'll use the max element width as width, for now
            int maxwidth = 0;
            for (size_t i = 0; i < count; i++){
                UiElement* element = menu->elements[i];

                if(element->type == UI_TYPE_MENU){
                    int subbake = mqui_as_menu(element)->bakeType;
                    if(subbake == BAKE_TYPE_SET_X_SPACING || subbake == BAKE_TYPE_SET_Y_SPACING){
                        printf("Baking recursive menus with set spacing is unsupported %d\n", __LINE__);
                        exit(-1);
                    }

                    Vec4I subbord = borders;
                    subbord.t -= ySum;
                    printf("ysum %d\n", ySum);
                    printf("baking sub with %d %d %d %d\n", subbord.l, subbord.r, subbord.t, subbord.b);
                    Menu_Bake(mqui_as_menu(element), subbord, (Vec4I){0,0,0,0}, mqui_as_menu(element)->bakeType, false);
                    printf(("post-bake %d %d %d %d\n"), element->position.x, element->position.y, UiElement_Size(element).x, UiElement_Size(element).y);
                    // exit(0);
                    maxwidth = max(maxwidth, UiElement_Size(element).x);
                }
                else
                    maxwidth = max(maxwidth, UiElement_Size(element).x);
            }
            borders.r = maxwidth;

            break; }

        case BAKE_TYPE_SET_X_SPACING:
            // With a given spacing, we need to calculate the bounding box (borders).
            xSpace = borders.r;

            borders.l = 0;
            borders.r = (count-1) * xSpace + xSum + border_offset.l + border_offset.l;

            // We'll use the max element width as width, for now
            int maxHeight = 0;
            for (size_t i = 0; i < count; i++){
                UiElement* element = menu->elements[i];

                if(element->type == UI_TYPE_MENU){
                    printf("Baking recursive menus with set spacing is unsupported\n");
                    exit(-1);
                }

                else
                    maxHeight = max(maxHeight, UiElement_Size(menu->elements[i]).y);
            }
            borders.t = 0;
            borders.b = maxHeight;

            break;
    }


    switch (baketype)
    {
        case BAKE_TYPE_SET_Y_SIZE: 
        case BAKE_TYPE_SET_Y_SPACING: {
            int y = border_offset.t;
            for(int i = 0; i < count; i++){

                UiElement* element = menu->elements[i];
                Vec2I elSize = UiElement_Size(element);
                int x = nonCenteredWidth/2 - elSize.x/2;
                int x2 = borderCenterX - nonCenteredWidth/2;
                element->position = (Vec2I) {.x = borders.l + border_offset.l + element->hcentered*x + centerInBorders*x2, .y = y + borders.t};
                if(element->type == UI_TYPE_BUTTON){
                    printf("YButt %d %d\n", x, borders.l);
                    // exit(0);
                }
                y += ySpace;
                y += elSize.y;
            }
            break; }
            
        case BAKE_TYPE_SET_X_SIZE:
        case BAKE_TYPE_SET_X_SPACING:
            int x = border_offset.l;
            for(int i = 0; i < count; i++){

                UiElement* element = menu->elements[i];
                Vec2I elSize = UiElement_Size(element);
                int y = nonCenteredHeight/2 - elSize.y/2;
                int y2 = borderCenterY - nonCenteredHeight/2;
                element->position = (Vec2I) {.y = borders.t + border_offset.t + element->vcentered*y + centerInBorders*y2, .x = x + borders.l};
                if(element->type == UI_TYPE_BUTTON){
                    printf("XButt %d %d\n", x, borders.l);
                    // exit(0);
                }
                x += xSpace;
                x += elSize.x;
            }
            break;
            
        default:
            exit(-1);
    }

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

void Menu_Draw(UiMenu* menu){

    if(!menu)
        return;

    RectI rect;
    rect = (RectI) {0, 0, menu->background_texture.width, menu->background_texture.height};
    DrawTexture(menu->background_texture, menu->position.x, menu->position.y, WHITE);

    for(int i = 0; i < sb_count(menu->elements); i++){
        UiElement* element = menu->elements[i];

        switch(menu->elements[i]->type){
            case UI_TYPE_BUTTON:
                Button_Draw(menu, mqui_as_button(element));
                break;
            case UI_TYPE_LABEL:
                Label_Draw(menu, mqui_as_label(element));
                break;
            case UI_TYPE_SPRITE:
                Sprite_Draw(menu, mqui_as_sprite(element));
                break;

            case UI_TYPE_MATCH:
                break;

            case UI_TYPE_MENU:
                printf("Drawing submenu [%p] at %d %d %d %d\n", element,
                    element->position.x, element->position.y,
                    UiElement_Size(element).x, UiElement_Size(element).y
                );
                Menu_Draw(mqui_as_menu(element));
                break;

            case UI_TYPE_LIST:
                break;
            
            case UI_TYPE_PREVIEW:
                break;
            
            default:
                printf("ERROR (%s %d): UNKNOWN UI TYPE %d\n", __FILE__, __LINE__, menu->elements[i]->type);
                exit(2);
        }
    }
}

void Menu_Free(UiMenu* menu){
    sb_free(menu->elements);
}

UiMenu* Menu_New(int baketype){
    UiMenu* m = malloc(sizeof(UiMenu));
    m->position = (Vec2I) {0, 0};
    m->bakeType = baketype;
    m->on_cancel.function = NULL;
    m->p1focused = 0;
    m->p2focused = 0;
    m->elements = NULL;
    m->type = UI_TYPE_MENU;
    m->hcentered = 0;
    return m;
}