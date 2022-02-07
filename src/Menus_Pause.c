#include "Menus_Pause.h"
#include "Ui_Types.h"
#include "main.h"
#include "TestMenus.h"

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
    Match_End(Ui_PopFocus());

    printf("Now %d on focus stack\n", sb_count(Ui_GetFocusStack()));
    MakeMainMenu(gRenderer);
}

UiMenu* MakePauseMenu(SDL_Renderer* gRenderer){

    printf("Paused\n");

    SDL_Surface* surf;

    UiMenu* menu = malloc(sizeof(UiMenu));
    menu->type = UI_TYPE_MENU;
    menu->background_surface = NULL;
    menu->background_texture = NULL;
    menu->p1focused = NULL;
    menu->p2focused = NULL;
    menu->on_cancel;
    // menu->on_cancel = (CallbackFunction){cb_back};
    menu->elements = NULL;

    // Background should be the size of the screen and have a negative position so that it covers the whole screen
    menu->background_surface = IMG_Load("Graphics/Ui/pausebg.png");
    menu->background_texture = SDL_CreateTextureFromSurface(gRenderer, menu->background_surface);

    int i = 0;
    UiButton* title         = Label_New (gRenderer, menu, "--Pause--"); i++;
    UiButton* butt_continue = Button_New(gRenderer, menu, "Continue", 20); i++;
    UiButton* butt_set_conts= Button_New(gRenderer, menu, "Set Controls", 20); i++;
    UiButton* butt_view_acts= Button_New(gRenderer, menu, "View Commands", 20); i++;
    UiButton* butt_char_sele= Button_New(gRenderer, menu, "Character Select", 20); i++;
    UiButton* butt_main_menu= Button_New(gRenderer, menu, "Main Menu", 20); i++;

    menu->on_cancel = (CallbackFunction){.function=cb_pause_menu_continue};
    butt_continue->on_confirm = (CallbackFunction){.function=cb_pause_menu_continue};
    butt_main_menu->on_confirm = (CallbackFunction){.function=cb_pause_menu_main};

    butt_continue->down = butt_set_conts;
    butt_set_conts->down = butt_view_acts;
    butt_view_acts->down = butt_char_sele;
    butt_char_sele->down = butt_main_menu;

    butt_set_conts->up = butt_continue;
    butt_view_acts->up = butt_set_conts;
    butt_char_sele->up = butt_view_acts;
    butt_main_menu->up = butt_char_sele;
    

    menu->p1focused = butt_continue;

    Ui_PushFocus(menu);
    Menu_Bake(gRenderer, menu, (Vector4){0,640,50,480-50}, (Vector4){.l=0, .r=0, .t=0, .b=0}, BAKE_TYPE_SET_SIZE, true);
    return menu;


}
