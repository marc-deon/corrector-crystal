#include "TestMenus.h"
#include <stdlib.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <SDL2/SDL_ttf.h>
#include "main.h"

// SDL_Texture* mainMenuButtonBg;
SDL_Texture* mainMenuButtonTextTextures[5];
SDL_Rect mainMenuButtonRects[5];
SDL_Surface* mainMenuButtonTextSurfaces[5];
SDL_Texture* mainMenuGrillTexture;
SDL_Rect mainMenuGrillRect;

#pragma region Button Callbacks
void cb_arcade(){
    MakeArcadeMenu(gRenderer);
}

void cb_versus(){
    MakeVersusMenu();
}

void cb_training(){
    MakeTrainingMenu();
}

void cb_replays(){
    MakeReplaysMenu();
}

void cb_options(){
    MakeOptionsMenu();
}

void cb_quit(){
    Ui_CloseTop();
    stopGame = 1;
}

void cb_back(){
    Ui_CloseTop();
}

int playerCharaIndices[2] = {-1, -1};

void cb_arcade_back(CallbackInfo info){

    if (playerCharaIndices[info.player] == -1){
        Ui_CloseTop();
    }

    playerCharaIndices[info.player] = -1;
}

#pragma endregion

void TransitionToMatch(){

}

void cb_character_select(CallbackInfo info){
    printf("Player %d selected character %d\n", info.player, info.arg);
    playerCharaIndices[info.player] = info.arg;

    if(info.player == 0){
        Ui_GetTopFocus()->p1focused = NULL;
    }else if (info.player == 1){
        Ui_GetTopFocus()->p2focused = NULL;
    }

    if (playerCharaIndices[0] != -1 && playerCharaIndices[1] != -1){
        while(Ui_GetTopFocus())
            Ui_PopFocus();
        Ui_Match* m = GameInit(playerCharaIndices[0], playerCharaIndices[1]);
        Ui_PushFocus(m);
    }
}

// TODO: Add rects for portraits on left and right, read textures from file as neccesary
UiMenu* MakeArcadeMenu(SDL_Renderer* gRenderer){
    playerCharaIndices[0] = -1;
    playerCharaIndices[1] = -1;
    UiMenu* menu = malloc(sizeof(UiMenu));
    menu->background_surface = NULL;
    menu->background_texture = NULL;
    menu->elements = 0;
    menu->on_cancel = (CallbackFunction){cb_arcade_back};

    menu->background_surface = IMG_Load("Graphics/Ui/menubg.png");
    menu->background_texture = SDL_CreateTextureFromSurface(gRenderer, menu->background_surface);

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


    SDL_Surface* surface_norm = IMG_Load("Graphics/Ui/charaselebutton-base1.png");
    SDL_Texture* texture_norm = SDL_CreateTextureFromSurface(gRenderer, surface_norm);
    SDL_Surface* surface_p1sel = IMG_Load("Graphics/Ui/charaselebutton-base2.png");
    SDL_Texture* texture_p1sel = SDL_CreateTextureFromSurface(gRenderer, surface_p1sel);
    SDL_Surface* surface_p2sel = IMG_Load("Graphics/Ui/charaselebutton-base3.png");
    SDL_Texture* texture_p2sel = SDL_CreateTextureFromSurface(gRenderer, surface_p2sel);
    SDL_Surface* surface_bothsel = IMG_Load("Graphics/Ui/charaselebutton-base4.png");
    SDL_Texture* texture_bothsel = SDL_CreateTextureFromSurface(gRenderer, surface_bothsel);


    UiButton* butt_shoto    = Button_New(gRenderer, menu, "", 32);
    UiSprite* sprite_shoto  = Sprite_New(gRenderer, menu, "Graphics/Ui/charaselebutton-1.png");
    UiButton* butt_guile    = Button_New(gRenderer, menu, "", 32);
    UiSprite* sprite_guile  = Sprite_New(gRenderer, menu, "Graphics/Ui/charaselebutton-2.png");
    UiButton* butt_cannon   = Button_New(gRenderer, menu, "", 32);
    UiSprite* sprite_cannon = Sprite_New(gRenderer, menu, "Graphics/Ui/charaselebutton-3.png");
    UiButton* butt_grappler = Button_New(gRenderer, menu, "", 32);
    UiSprite* sprite_grappler   = Sprite_New(gRenderer, menu, "Graphics/Ui/charaselebutton-4.png");
    UiButton* butt_random   = Button_New(gRenderer, menu, "", 32);
    UiSprite* sprite_random = Sprite_New(gRenderer, menu, "Graphics/Ui/charaselebutton-0.png");
    UiButton* butt_axl      = Button_New(gRenderer, menu, "", 32);
    UiSprite* sprite_axl    = Sprite_New(gRenderer, menu, "Graphics/Ui/charaselebutton-5.png");
    UiButton* butt_pixie    = Button_New(gRenderer, menu, "", 32);
    UiSprite* sprite_pixie  = Sprite_New(gRenderer, menu, "Graphics/Ui/charaselebutton-6.png");
    UiButton* butt_puppet   = Button_New(gRenderer, menu, "", 32);
    UiSprite* sprite_puppet = Sprite_New(gRenderer, menu, "Graphics/Ui/charaselebutton-7.png");
    UiButton* butt_stance   = Button_New(gRenderer, menu, "", 32);
    UiSprite* sprite_stance = Sprite_New(gRenderer, menu, "Graphics/Ui/charaselebutton-8.png");

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

    for (int i = -1; i <= 1; i++){
        xs[i+1] = midx + i*(buttsize+space) - buttsize/2;
    }
    for (int i = -1; i <= 1; i++){
        ys[i+1] = midy + i*(buttsize+space) - buttsize/2;
    }

    for(int x = 0; x < 3; x++){
        for(int y = 0; y < 3; y++){
            butts[y][x]->position.x = xs[x];
            butts[y][x]->position.y = ys[y];
            sprites[y][x]->position.x = xs[x] + 2;
            sprites[y][x]->position.y = ys[y] + 2;
            butts[y][x]->surface_normal = surface_norm;
            butts[y][x]->texture_normal = texture_norm;
            butts[y][x]->texture_p1hover = texture_p1sel;
            butts[y][x]->surface_p1hover = surface_p1sel;
            butts[y][x]->texture_p2hover = texture_p2sel;
            butts[y][x]->surface_p2hover = surface_p2sel;
            butts[y][x]->texture_bothhover = texture_bothsel;
            butts[y][x]->surface_bothhover = surface_bothsel;

            butts[y][x]->left  = x > 0 ? mqui_as_element(butts[y][x-1]) : 0;
            butts[y][x]->right = x < 2 ? mqui_as_element(butts[y][x+1]) : 0;
            butts[y][x]->up    = y > 0 ? mqui_as_element(butts[y-1][x]) : 0;
            butts[y][x]->down  = y < 2 ? mqui_as_element(butts[y+1][x]) : 0;

            butts[y][x]->on_confirm = (CallbackFunction){cb_character_select, (CallbackInfo){0, 0, 3*y+x}};
        }
    }

    Ui_PushFocus(menu);
    return menu;
}

UiMenu* MakeVersusMenu(SDL_Renderer* gRenderer){
    UiMenu* menu = malloc(sizeof(UiMenu));
    menu->background_surface = NULL;
    menu->background_texture = NULL;
    menu->elements = 0; 

    return Ui_PushFocus(menu);
}

UiMenu* MakeTrainingMenu(SDL_Renderer* gRenderer){
    UiMenu* menu = malloc(sizeof(UiMenu));
    menu->background_surface = NULL;
    menu->background_texture = NULL;
    menu->elements = 0;

    return Ui_PushFocus(menu);
}

UiMenu* MakeReplaysMenu(SDL_Renderer* gRenderer){
    UiMenu* menu = malloc(sizeof(UiMenu));
    menu->background_surface = NULL;
    menu->background_texture = NULL;
    menu->elements = 0;
}

UiMenu* MakeOptionsMenu(SDL_Renderer* gRenderer){
    UiMenu* menu = malloc(sizeof(UiMenu));
    menu->background_surface = NULL;
    menu->background_texture = NULL;
    menu->elements = 0;

    return Ui_PushFocus(menu);
}

UiMenu* MakeMainMenu(SDL_Renderer* gRenderer){
    SDL_Surface* surf;

    UiMenu* menu = malloc(sizeof(UiMenu));
    menu->background_surface = NULL;
    menu->background_texture = NULL;
    menu->p1focused = NULL;
    menu->p2focused = NULL;
    menu->on_cancel = (CallbackFunction){cb_back};
    menu->elements = NULL;

    menu->background_surface = IMG_Load("Graphics/Ui/mainbg.png");
    menu->background_texture = SDL_CreateTextureFromSurface(gRenderer, menu->background_surface);

    // TODO(#27): Find a way to:
    // 1) Automatically space these with each other and with the borders
    // 2) Center-align the labels
    // Maybe use a Menu_Bake()?
    // Maybe find the bounding box of everything that's not centered, then use that to figure out where everything that is centered goes?
    int i = 0;
    UiElement* title         = mqui_as_element(Label_New (gRenderer, menu, "--Main--")); i++;
    UiElement* butt_arcade   = mqui_as_element(Button_New(gRenderer, menu, "Arcade"  , 32)); i++;
    UiElement* butt_versus   = mqui_as_element(Button_New(gRenderer, menu, "Versus"  , 32)); i++;
    UiElement* butt_training = mqui_as_element(Button_New(gRenderer, menu, "Training", 32)); i++;
    UiElement* butt_replays  = mqui_as_element(Button_New(gRenderer, menu, "Replays" , 32)); i++;
    UiElement* butt_options  = mqui_as_element(Button_New(gRenderer, menu, "Options" , 32)); i++;
    UiElement* butt_quit     = mqui_as_element(Button_New(gRenderer, menu, "quit"    , 32)); i++;
    
    butt_arcade->down = butt_versus;
    mqui_as_button(butt_arcade)->on_confirm = (CallbackFunction){cb_arcade};    

    butt_versus->up = butt_arcade;
    butt_versus->down = butt_training;
    mqui_as_button(butt_versus)->on_confirm = (CallbackFunction){cb_versus};    
    
    butt_training->up = butt_versus;
    butt_training->down = butt_replays;
    mqui_as_button(butt_training)->on_confirm = (CallbackFunction){cb_training};    

    butt_replays->up = butt_training;
    butt_replays->down = butt_options;
    mqui_as_button(butt_replays)->on_confirm = (CallbackFunction){cb_replays};    

    butt_options->up = butt_replays;
    butt_options->down = butt_quit;
    mqui_as_button(butt_options)->on_confirm = (CallbackFunction){cb_options};
    
    butt_quit->up = butt_options;
    mqui_as_button(butt_quit)->on_confirm = (CallbackFunction){cb_quit};

    menu->p1focused = butt_arcade;
    Ui_PushFocus(menu);
    Menu_Bake(gRenderer, menu, (Vector4){0,640,0,480}, (Vector4){.l=4, .r=4, .t=4, .b=4}, BAKE_TYPE_SET_SIZE, false);
    return menu;
}
