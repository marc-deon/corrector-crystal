#include "Ui_Base.h"
#include "stretchy_buffer.h"
#include "Ui_Types.h"
#include "stdio.h"

UiMenu** _ui_focus_stack = 0;

// Use with caution!
void** Ui_GetFocusStack(){
    return _ui_focus_stack;
}

UiMenu* Ui_GetTopFocus(){
    return sb_count(_ui_focus_stack) ? sb_last(_ui_focus_stack) : 0;
}

UiMenu* Ui_PopFocus(){
    return sb_count(_ui_focus_stack) ? sb_pop(_ui_focus_stack) : 0;
}

UiMenu* Ui_PushFocus(UiMenu* focus){
    return sb_push(_ui_focus_stack, focus);
}

void Ui_CloseTop(){
    UiMenu* frame = Ui_PopFocus();
    for(int i = 0; i < sb_count(frame->elements); i++){
        // free textures
        // free element
        switch(frame->elements[i]->type){
            case UI_TYPE_BUTTON:
                Button_Free(mqui_as_button(frame->elements[i]));
                break;
            case UI_TYPE_LABEL:
                Label_Free(mqui_as_label(frame->elements[i]));
                break;
        }

    }
    sb_free(frame->elements);
    free(frame);
}

Vec2I UiElement_Size(UiElement* elem){
    Vec2I size = (Vec2I){-1, -1};
    switch(elem->type){
        case UI_TYPE_BUTTON:
            // printf("Found UI_TYPE_BUTTON\n");
            size = Button_Size(mqui_as_button(elem));
            break;
        case UI_TYPE_LABEL:
            // printf("Found UI_TYPE_LABEL\n");
            size = Label_Size(mqui_as_label(elem));
            break;
        case UI_TYPE_MENU:
            // printf("Found UI_TYPE_MENU\n");
            size = Menu_Size(mqui_as_menu(elem));
            break;
            
        case UI_TYPE_LIST:
            printf("list size\n");
            break;

        case UI_TYPE_PREVIEW:
            break;

        default:
            printf("Element type %d does not have a size", elem->type);
            exit(-1);
            break;
    }

    // printf("calculated size is %d %d type %d\n", size.x, size.y, elem->type);
    return size;
}
