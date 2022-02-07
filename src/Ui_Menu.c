#include "Ui_Base.h"
#include "Ui_Types.h"
#include "stretchy_buffer.h"
#include "CC_Audio.h"

#define max(a,b) (a > b ? a : b)

// Funfact: Bake is optional! If you want a "floating" layout, don't use it.
// When baketype is BAKE_TYPE_SET_SPACING, the b component of borders is used as the spacing
// centerInBorders only centers horizontally (for now?)
void Menu_Bake(SDL_Renderer* ren, UiMenu* menu, Vector4 borders, Vector4 border_offset, int baketype, int centerInBorders){

    // TODO(#28): Make an actual Menu type, store the borders on that, and draw children relative to it

    switch(baketype){
        // With a given total size, we need to calculate the spacing.
        case BAKE_TYPE_SET_SIZE:
            int ySum = 0;
            int ySpace;
            int count = sb_count(menu->elements);
            int nonCenteredWidth = 0;

            for(int i = 0; i < count; i++){
                UiElement* element = menu->elements[i];
                Vector2 elSize = UiElement_Size(ren, element);
                ySum += elSize.y;
                nonCenteredWidth = max(nonCenteredWidth, (!element->hcentered) * elSize.x);
            }

            ySpace = ((borders.b - borders.t - border_offset.b - border_offset.t) - ySum) / (count-1);

            int borderCenterX = (borders.r - borders.l + border_offset.l - border_offset.r)/2;

            int y = border_offset.t;
            for(int i = 0; i < count; i++){

                UiElement* element = menu->elements[i];
                Vector2 elSize = UiElement_Size(ren, element);
                int x = nonCenteredWidth/2 - elSize.x/2;
                int x2 = borderCenterX - nonCenteredWidth/2;
                element->position = (Vector2) {.x = borders.l + border_offset.l + element->hcentered*x + centerInBorders*x2, .y = y + borders.t};
                y += ySpace;
                y += elSize.y;
            }

            break;
        case BAKE_TYPE_SET_SPACING:
            // With a given spacing, we need to calculate the bounding box (borders).
            puts("BAKE_TYPE_SET_SPACING Not implemented\n");
            exit(1);
            break;
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
            // CC_Audio_Play_SFX(kirins[kiringtone_current]);
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

void Menu_Draw(SDL_Renderer* ren, UiMenu* menu){

    if(!menu)
        return;

    if(menu->background_surface){
        SDL_Rect rect;
        rect = (SDL_Rect) {0, 0, menu->background_surface->w, menu->background_surface->h};
        SDL_RenderCopy(ren, menu->background_texture, NULL, &rect);
    }

    for(int i = 0; i < sb_count(menu->elements); i++){
        switch(menu->elements[i]->type){
            case UI_TYPE_BUTTON:
                Button_Draw(ren, menu, mqui_as_button(menu->elements[i]));
                break;
            case UI_TYPE_LABEL:
                Label_Draw(ren, menu, mqui_as_label(menu->elements[i]));
                break;
            case UI_TYPE_SPRITE:
                Sprite_Draw(ren, menu, mqui_as_sprite(menu->elements[i]));
                break;

            case UI_TYPE_MATCH:
                break;

            case UI_TYPE_MENU:
                break;
            
            default:
                printf("ERROR: UNKNOWN UI TYPE %d\n", menu->elements[i]->type);
                exit(2);
        }
    }
}

void Menu_Free(UiMenu* menu){
    SDL_FreeSurface(menu->background_surface);
    SDL_DestroyTexture(menu->background_texture);
    sb_free(menu->elements);
}