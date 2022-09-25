#include "TestMenus.h"
#include <stdlib.h>
#include "main.h"
#include <raylib.h>

Texture mainMenuButtonTextTextures[5];
RectI mainMenuButtonRects[5];
Texture mainMenuButtonTextSurfaces[5];
Texture mainMenuGrillTexture;
RectI mainMenuGrillRect;

#pragma region Button Callbacks
void cb_arcade() {
    MakeArcadeMenu();
}

void cb_versus() {
    MakeVersusMenu();
}

void cb_training() {
    MakeTrainingMenu();
}

void cb_replays() {
    MakeReplaysMenu();
}

void cb_options() {
    MakeOptionsMenu();
}

void cb_quit() {
    Ui_CloseTop();
    stopGame = 1;
}

void cb_back() {
    Ui_CloseTop();
}

int playerCharaIndices[2] = {-1, -1};

void cb_arcade_back(CallbackInfo info) {

    if (playerCharaIndices[info.player] == -1) {
        Ui_CloseTop();
    }

    playerCharaIndices[info.player] = -1;
}

#pragma endregion

void TransitionToMatch() {

}

void cb_character_select(CallbackInfo info) {
    printf("Player %d selected character %d\n", info.player, info.arg);
    playerCharaIndices[info.player] = info.arg;

    if(info.player == 0) {
        Ui_GetTopFocus()->p1focused = NULL;
    }else if (info.player == 1) {
        Ui_GetTopFocus()->p2focused = NULL;
    }

    if (playerCharaIndices[0] != -1 && playerCharaIndices[1] != -1) {
        while(Ui_GetTopFocus())
            Ui_PopFocus();
        UiMenu* m = mqui_as_menu(GameInit(playerCharaIndices[0], playerCharaIndices[1]));
        Ui_PushFocus(m);
    }
}

// TODO: Add rects for portraits on left and right, read textures from file as neccesary
UiMenu* MakeArcadeMenu() {
    playerCharaIndices[0] = -1;
    playerCharaIndices[1] = -1;
    UiMenu* menu = Menu_New(BAKE_TYPE_FLOATING);
    menu->elements = 0;
    menu->on_cancel = (CallbackFunction) {cb_arcade_back};

    menu->background_texture = LoadTexture("Graphics/Ui/menubg.png");

    /*
        Shoto    Guile  Cannon
        Grappler   ?    Axl
        Pixie    Puppet stance

    */

    const int buttsize = 64;
    const int midx = 640/2;
    const int midy = 480/2;
    const int space = 8;
    int xs[3];
    int ys[3];


    Texture surface_norm = LoadTexture("Graphics/Ui/charaselebutton-base1.png");
    Texture surface_p1sel = LoadTexture("Graphics/Ui/charaselebutton-base2.png");
    Texture surface_p2sel = LoadTexture("Graphics/Ui/charaselebutton-base3.png");
    Texture surface_bothsel = LoadTexture("Graphics/Ui/charaselebutton-base4.png");


    UiButton* butt_shoto    = Button_New("", 32);
    add_element(menu, butt_shoto);
    UiSprite* sprite_shoto  = Sprite_New(menu, "Graphics/Ui/charaselebutton-1.png");
    UiButton* butt_guile    = Button_New("", 32);
    add_element(menu, butt_guile);
    UiSprite* sprite_guile  = Sprite_New(menu, "Graphics/Ui/charaselebutton-2.png");
    UiButton* butt_cannon   = Button_New("", 32);
    add_element(menu, butt_cannon);
    UiSprite* sprite_cannon = Sprite_New(menu, "Graphics/Ui/charaselebutton-3.png");
    UiButton* butt_grappler = Button_New("", 32);
    add_element(menu, butt_grappler);
    UiSprite* sprite_grappler   = Sprite_New(menu, "Graphics/Ui/charaselebutton-4.png");
    UiButton* butt_random   = Button_New("", 32);
    add_element(menu, butt_random);
    UiSprite* sprite_random = Sprite_New(menu, "Graphics/Ui/charaselebutton-0.png");
    UiButton* butt_axl      = Button_New("", 32);
    add_element(menu, butt_axl);
    UiSprite* sprite_axl    = Sprite_New(menu, "Graphics/Ui/charaselebutton-5.png");
    UiButton* butt_pixie    = Button_New("", 32);
    add_element(menu, butt_pixie);
    UiSprite* sprite_pixie  = Sprite_New(menu, "Graphics/Ui/charaselebutton-6.png");
    UiButton* butt_puppet   = Button_New("", 32);
    add_element(menu, butt_puppet);
    UiSprite* sprite_puppet = Sprite_New(menu, "Graphics/Ui/charaselebutton-7.png");
    UiButton* butt_stance   = Button_New("", 32);
    add_element(menu, butt_stance);
    UiSprite* sprite_stance = Sprite_New(menu, "Graphics/Ui/charaselebutton-8.png");

    UiButton* butts[3][3] = {
        {butt_shoto, butt_guile, butt_cannon},
        {butt_grappler, butt_random, butt_axl},
        {butt_pixie, butt_puppet, butt_stance}
    };

    UiSprite* sprites[3][3] = {
        {sprite_shoto, sprite_guile, sprite_cannon},
        {sprite_grappler, sprite_random, sprite_axl},
        {sprite_pixie, sprite_puppet, sprite_stance}
    };

    menu->p1focused = mqui_as_element(butt_puppet);
    menu->p2focused = mqui_as_element(butt_puppet);

    for (int i = -1; i <= 1; i++) {
        xs[i+1] = midx + i*(buttsize+space) - buttsize/2;
    }
    for (int i = -1; i <= 1; i++) {
        ys[i+1] = midy + i*(buttsize+space) - buttsize/2;
    }

    for(int x = 0; x < 3; x++) {
        for(int y = 0; y < 3; y++) {
            butts[y][x]->position.x = xs[x];
            butts[y][x]->position.y = ys[y];
            sprites[y][x]->position.x = xs[x] + 2;
            sprites[y][x]->position.y = ys[y] + 2;
            butts[y][x]->texture_normal = surface_norm;
            butts[y][x]->texture_p1hover = surface_p1sel;
            butts[y][x]->texture_p2hover = surface_p2sel;
            butts[y][x]->texture_bothhover = surface_bothsel;

            butts[y][x]->left  = x > 0 ? mqui_as_element(butts[y][x-1]) : 0;
            butts[y][x]->right = x < 2 ? mqui_as_element(butts[y][x+1]) : 0;
            butts[y][x]->up    = y > 0 ? mqui_as_element(butts[y-1][x]) : 0;
            butts[y][x]->down  = y < 2 ? mqui_as_element(butts[y+1][x]) : 0;

            butts[y][x]->on_confirm = (CallbackFunction) {cb_character_select, (CallbackInfo) {0, 0, 3*y+x}};
        }
    }

    Ui_PushFocus(menu);
    return menu;
}

UiMenu* MakeVersusMenu() {
    UiMenu* menu = Menu_New(BAKE_TYPE_FLOATING);

    return Ui_PushFocus(menu);
}

UiMenu* MakeTrainingMenu() {
    UiMenu* menu = Menu_New(BAKE_TYPE_FLOATING);

    return Ui_PushFocus(menu);
}

UiMenu* MakeReplaysMenu() {
    UiMenu* menu = Menu_New(BAKE_TYPE_SET_Y_SPACING);
}

UiMenu* MakeOptionsMenu() {
    UiMenu* menu = Menu_New(BAKE_TYPE_SET_Y_SPACING);

    return Ui_PushFocus(menu);
}

UiMenu* MakeMainMenu() {
    Texture surf;

    UiMenu* menu = Menu_New(BAKE_TYPE_SET_Y_SIZE);
    menu->p1focused = NULL;
    menu->p2focused = NULL;
    menu->on_cancel = (CallbackFunction) {cb_back};
    menu->elements = NULL;

    menu->background_texture = LoadTexture("Graphics/Ui/mainbg.png");

    // TODO(#27): Find a way to:
    // 1) Automatically space these with each other and with the borders
    // 2) Center-align the labels
    // Maybe use a Menu_Bake()?
    // Maybe find the bounding box of everything that's not centered, then use that to figure out where everything that is centered goes?
    int i = 0;
    UiElement* title         = mqui_as_element(Label_New (menu, "--Main--")); i++;
    UiElement* butt_arcade   = mqui_as_element(Button_New("Arcade"  , 32)); i++;
    add_element(menu, butt_arcade);
    UiElement* butt_versus   = mqui_as_element(Button_New("Versus"  , 32)); i++;
    add_element(menu, butt_versus);
    UiElement* butt_training = mqui_as_element(Button_New("Training", 32)); i++;
    add_element(menu, butt_training);
    UiElement* butt_replays  = mqui_as_element(Button_New("Replays" , 32)); i++;
    add_element(menu, butt_replays);
    UiElement* butt_options  = mqui_as_element(Button_New("Options" , 32)); i++;
    add_element(menu, butt_options);
    UiElement* butt_quit     = mqui_as_element(Button_New("quit"    , 32)); i++;
    add_element(menu, butt_quit);
    
    butt_arcade->down = butt_versus;
    mqui_as_button(butt_arcade)->on_confirm = (CallbackFunction) {cb_arcade};    

    butt_versus->up = butt_arcade;
    butt_versus->down = butt_training;
    mqui_as_button(butt_versus)->on_confirm = (CallbackFunction) {cb_versus};    
    
    butt_training->up = butt_versus;
    butt_training->down = butt_replays;
    mqui_as_button(butt_training)->on_confirm = (CallbackFunction) {cb_training};    

    butt_replays->up = butt_training;
    butt_replays->down = butt_options;
    mqui_as_button(butt_replays)->on_confirm = (CallbackFunction) {cb_replays};    

    butt_options->up = butt_replays;
    butt_options->down = butt_quit;
    mqui_as_button(butt_options)->on_confirm = (CallbackFunction) {cb_options};
    
    butt_quit->up = butt_options;
    mqui_as_button(butt_quit)->on_confirm = (CallbackFunction) {cb_quit};

    menu->p1focused = butt_arcade;
    Ui_PushFocus(menu);

    menu->size = (Vec2I) {1280, 720};
    menu->margin = (Vec4I) {4, 4, 4, 4};
    Menu_Bake(menu, false);
    return menu;
}
