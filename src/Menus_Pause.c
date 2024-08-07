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

void cb_pause_menu_continue(CallbackInfo info) {
    UiMenu* top = Ui_PopFocus();
    Menu_Free(top);
    currentMatch.paused = false;
}

void cb_pause_menu_main(CallbackInfo info) {
    UiMenu* top = Ui_PopFocus();
    Menu_Free(top);
    Match_End((Match*) Ui_PopFocus());

    printf("Now %d on focus stack\n", sb_count(Ui_GetFocusStack()));
    MakeMainMenu();
}

UiMenu* MakePauseMenu() {

    printf("Paused\n");

    Texture surf;

    UiMenu* menu = Menu_New(BAKE_TYPE_SET_Y_SIZE);
    menu->type = UI_TYPE_MENU;
    menu->p1focused = NULL;
    menu->p2focused = NULL;
    menu->on_cancel;
    // menu->on_cancel = (CallbackFunction) {cb_back};
    menu->elements = NULL;

    // Background should be the size of the screen and have a negative position so that it covers the whole screen
    menu->background_texture = LoadTexture("Graphics/Ui/pausebg.png");
    
    int i = 0;
    Label_New (menu, "--Pause--"); i++;
    UiButton* butt_continue = Button_New("Continue", 20); i++;
    add_element(menu, butt_continue);
    UiButton* butt_set_conts= Button_New("Set Controls", 20); i++;
    add_element(menu, butt_set_conts);
    UiButton* butt_view_acts= Button_New("View Commands", 20); i++;
    add_element(menu, butt_view_acts);
    UiButton* butt_char_sele= Button_New("Character Select", 20); i++;
    add_element(menu, butt_char_sele);
    UiButton* butt_main_menu= Button_New("Main Menu", 20); i++;
    add_element(menu, butt_main_menu);

    menu->on_cancel = (CallbackFunction) {.function=cb_pause_menu_continue};
    butt_continue->on_confirm = (CallbackFunction) {.function=cb_pause_menu_continue};
    butt_main_menu->on_confirm = (CallbackFunction) {.function=cb_pause_menu_main};

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
    
    menu->bakeType = BAKE_TYPE_SET_Y_SIZE;
    menu->size = (Vec2I) {VIRT_SCREEN_SIZE_X, VIRT_SCREEN_SIZE_Y};
    menu->margin = (Vec4I) { 5, 5, 5, 5 };

    Menu_Bake(menu, true);
    // Menu_Bake(menu, (Vec4I) {0,VIRT_SCREEN_SIZE_X,50,VIRT_SCREEN_SIZE_Y-50}, (Vec4I) {.l=0, .r=0, .t=0, .b=0}, BAKE_TYPE_SET_Y_SIZE, true);
    return menu;


}
