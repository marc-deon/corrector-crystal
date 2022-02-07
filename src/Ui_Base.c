#include "Ui_Base.h"
#include "stretchy_buffer.h"
#include "Ui_Types.h"
#define max(a,b) (a > b ? a : b)

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

Vector2 UiElement_Size(SDL_Renderer* ren, UiElement* elem){
    Vector2 size = (Vector2){-1, -1};
    switch(elem->type){
        case UI_TYPE_BUTTON:
            size = Button_Size(ren, mqui_as_button(elem));
            break;
        case UI_TYPE_LABEL:
            size = Label_Size(ren, mqui_as_label(elem));
            break;
    }
    return size;
}
