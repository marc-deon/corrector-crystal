#include "Menus_Pause.h"
#include "Ui_Types.h"
#include "main.h"
#include "TestMenus.h"
#include <raylib.h>

/*
Continue
Set Controls
View Commands
Character Select
Main Menu
*/

void cb_pause_menu_continue(CallbackInfo info){
    UiMenu* top = Ui_PopFocus();
    Menu_Free(top);
    currentMatch.paused = false;
}

void cb_pause_menu_main(CallbackInfo info){
    UiMenu* top = Ui_PopFocus();
    Menu_Free(top);
    Match_End((Match*) Ui_PopFocus());

    printf("Now %d on focus stack\n", sb_count(Ui_GetFocusStack()));
    MakeMainMenu();
}

UiMenu* MakePauseMenu(){

    printf("Paused\n");

    Texture surf;

    UiMenu* menu = malloc(sizeof(UiMenu));
    menu->type = UI_TYPE_MENU;
    menu->p1focused = NULL;
    menu->p2focused = NULL;
    menu->on_cancel;
    // menu->on_cancel = (CallbackFunction){cb_back};
    menu->elements = NULL;

    // Background should be the size of the screen and have a negative position so that it covers the whole screen
    menu->background_texture = LoadTexture("Graphics/Ui/pausebg.png");
    
    // menu->background_texture = SDL_CreateTextureFromSurface(menu->background_texture);

    int i = 0;
    Label_New (menu, "--Pause--"); i++;
    UiButton* butt_continue = Button_New(menu, "Continue", 20); i++;
    UiButton* butt_set_conts= Button_New(menu, "Set Controls", 20); i++;
    UiButton* butt_view_acts= Button_New(menu, "View Commands", 20); i++;
    UiButton* butt_char_sele= Button_New(menu, "Character Select", 20); i++;
    UiButton* butt_main_menu= Button_New(menu, "Main Menu", 20); i++;

    menu->on_cancel = (CallbackFunction){.function=cb_pause_menu_continue};
    butt_continue->on_confirm = (CallbackFunction){.function=cb_pause_menu_continue};
    butt_main_menu->on_confirm = (CallbackFunction){.function=cb_pause_menu_main};

    butt_continue->down =  mqui_as_element(butt_set_conts);
    butt_set_conts->down = mqui_as_element(butt_view_acts);
    butt_view_acts->down = mqui_as_element(butt_char_sele);
    butt_char_sele->down = mqui_as_element(butt_main_menu);

    butt_set_conts->up = mqui_as_element(butt_continue);
    butt_view_acts->up = mqui_as_element(butt_set_conts);
    butt_char_sele->up = mqui_as_element(butt_view_acts);
    butt_main_menu->up = mqui_as_element(butt_char_sele);
    

    menu->p1focused = mqui_as_element(butt_continue);

    Ui_PushFocus(menu);
    
    Menu_Bake(menu, (Vec4I){0,VIRT_SCREEN_SIZE_X,50,VIRT_SCREEN_SIZE_Y-50}, (Vec4I){.l=0, .r=0, .t=0, .b=0}, BAKE_TYPE_SET_SIZE, true);
    return menu;


}
