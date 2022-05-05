#define ASAMIYA

#include <raylib.h>
#include <stdio.h>
#include "stretchy_buffer.h"
#include "Match.h"
#include "Fighter.h"
#include "Fighter_Read.h"
#include "Fighter_Write.h"
#include "Ui_Types.h"
#include "circular_buffer.h"
#include "stretchy_buffer.h"

#define MOUSEWHEEL_UP (1f)
#define MOUSEWHEEL_DOWN (-1f)

typedef struct {
    int itemCount;
    int focusIndex;  // Which item is currently hovered
    int scrollIndex; // Which item to begin drawing at
    int activeIndex; // Which item is currently active
    int maxHeight;
    char** items;

} ListViewInfo;

ListViewInfo shoppingList = {
    .itemCount   = -1,
    .focusIndex  = -1,
    .scrollIndex = -1,
    .activeIndex = -1,
    .maxHeight   = -1,
    .items = NULL
};

////////////////////
// Move to new files

typedef struct {
    ui_preamble

    void* itemSource;

} UiList;



UiList* List_New(){
    UiList* l = malloc(sizeof(UiList));
    l->position = (Vec2I) {0,0};
    l->id = uiidcount++;
    l->type = UI_TYPE_LIST;
    l->itemSource = NULL;
}

// Loop through every box:
//     If the nth box has a size of (0,0), swap it with the n+1th box.
//     Repeat loop until no nulls are found.
// Then while the last box has size (0,0), pop it.
//
// This is a little bit inefficient (it's basically bubble sort), but it's w/e.
// We're dealing with very small arrays here just on button press in editor.
void PruneBoxes(void** sb, enum boxtype t){

    bool foundNull = true;
    int loopTil = sb_count(sb)-1;

    while(foundNull){
        foundNull = false;
        for(int i = 0; i < loopTil; i++){

            // If size is (0,0)
            if( ((Hitbox*)(sb[i]))->rect.w == 0 && ((Hitbox*)(sb[i]))->rect.h == 0){
                // Swap with next
                void* temp = sb[i];
                sb[i] = sb[i+1];
                sb[i+1] = temp;

                // We might need to loop again
                foundNull = true;
            }
        }
        loopTil--;
    }

    // Pop trailing zero-sized boxes and free
    if(sb_count(sb) > 0){
        while(sb_last((Hitbox**)sb)->rect.w == 0 && sb_last((Hitbox**)sb)->rect.h == 0){
            switch(t) {
                case boxtype_hit:
                    free((Hitbox**)sb_pop(sb));
                    break;
                case boxtype_hurt:
                    free((Hurtbox**)sb_pop(sb));
                    break;
            }
        }
    }
}

Match currentMatch;

UiMenu* root;

void stopGame(){ /* Left intentionally blank */ }

void GameInit(){ /* Left intentionally blank */ }

void cb_SwapToActions(){
    printf("Swapping to Actions\n");
}

void cb_SwapToAnimations(){
    printf("Swapping to Animations\n");
}

void List_Draw(ListViewInfo* lvi, Vector2 offset){
    int height = 0;
    int y = 0;
    const int FONTSIZE = 20;

    for(int i = lvi->scrollIndex; (i < lvi->itemCount) && (height <= lvi->maxHeight); i++){
        Font f = GetFontDefault();
        char* text = lvi->items[i];
        Vector2 size = MeasureTextEx(f, text, FONTSIZE, 2);
        height += size.y;
        y += size.y;

        Color color = lvi->activeIndex == i ? GREEN : lvi->focusIndex == i ? WHITE : GRAY;

        if (height <= lvi->maxHeight){
            DrawText(lvi->items[i], offset.x, y + offset.y, FONTSIZE, color);
        }
    }
}

void cb_tab_act(){
    // Generate list info based on fighter actions
    shoppingList.itemCount = sb_count(asamiya_f->actions);
    shoppingList.focusIndex = 0;
    shoppingList.scrollIndex = 0;
    shoppingList.activeIndex = 0;
    // shoppingList.maxHeight 

    shoppingList.items = (char**) malloc(shoppingList.itemCount * sizeof(char*));

    for (size_t i = 0; i < shoppingList.itemCount; i++)
    {
        shoppingList.items[i] = (char*) malloc(TextLength(asamiya_f->actions[i]->name ) + 1);
        TextCopy(shoppingList.items[i], asamiya_f->actions[i]->name);
        // printf("[%s]\n", shoppingList.items[i]);
    }
    
    
    // Disable hitboxes button

}

void cb_tab_anim(){

}

void cb_alist_click(){

}

void cb_alist_scroll(int dir){
    printf("scroll\n");
    shoppingList.activeIndex = max(0, min(shoppingList.itemCount-1, shoppingList.activeIndex + dir));

}

void cb_preview_drag(){

}

void cb_tab_inspector(){

}

void cb_tab_hitbox(){

}

void cb_rlist_scroll(){

}

void SetupRaylib(){
    SetTraceLogLevel(LOG_NONE);
    SetTargetFPS(60);
    InitWindow(1280, 720, "Asamiya Character Creater");
}

void SetupData(){
    /* 0  */ // root;
    /* 1  */ UiMenu* leftPane;
    /* 2  */     UiMenu* leftTabs;
    /* 3  */         UiButton* buttAct;
    /* 4  */         UiButton* buttAnim;
    /* 5  */     UiList* aList;
    /* 6  */ UiPreview* preview;
    /* 7  */ UiMenu* rightPane;
    /* 8  */     UiMenu* rightTabs;
    /* 9  */         UiButton* buttInspector;
    /* 10 */         UiButton* buttHitboxes;
    /* 11 */     UiList* rList;


    root          = Menu_New(BAKE_TYPE_SET_X_SIZE);
    root->spacing = 5;
    leftPane      = Menu_New(BAKE_TYPE_SET_Y_SPACING);
    leftTabs      = Menu_New(BAKE_TYPE_SET_X_SIZE);
    buttAct       = Button_New("Act", 20);
    buttAnim      = Button_New("Anim", 20);
    aList         = List_New();
    preview       = Preview_New();
    rightPane     = Menu_New(BAKE_TYPE_SET_Y_SPACING);
    rightTabs     = Menu_New(BAKE_TYPE_SET_X_SIZE);
    buttInspector = Button_New("Inspector", 20);
    buttHitboxes  = Button_New("Hitboxes", 20);
    rList         = List_New();

    
    add_element(root, leftPane);
        add_element(leftPane, leftTabs);
            add_element(leftTabs, buttAct);
            add_element(leftTabs, buttAnim);
        add_element(leftPane, aList);
    add_element(root, preview);
    add_element(root, rightPane);
        add_element(rightPane, rightTabs);
            add_element(rightTabs, buttInspector);
            add_element(rightTabs, buttHitboxes);
        add_element(rightPane, rList);

    root->size = (Vec2I){1280, 720}; // Set size
    leftPane->size  = (Vec2I) {}; // Set spacing
    leftTabs->size  = (Vec2I) {ui_w(buttAct) + ui_w(buttAnim), ui_h(buttAct)}; // Set size
    rightPane->size = (Vec2I) {}; // Set spacing
    rightTabs->size = (Vec2I) {ui_w(buttAct) + ui_w(buttInspector), ui_h(buttHitboxes)}; // Set size

    Menu_Bake(root, true);
    // exit(0);
}


void SetupFighter(){
    Fighter* f = asamiya_f = Fighter_Create("fighterData/superman.jsonc");
    Fighter_GetPalette(f, 0);

    f->stateHistory = cb_init(f->stateHistory, MAX_REWIND);
    cb_push(f->stateHistory, (FighterState){});
    FighterState* fs = &cb_last(f->stateHistory);
    fs->stateFlags              = 0;
    fs->tempGravity.x           = 6969;
    fs->tempGravity.y           = 6969;
    fs->subframe                = 0;
    fs->animation = f->animations[0];
    fs->animation->currentFrame = 0;

    fs->jumps   = f->maxJumps;
    fs->health  = f->maxHealth;
    fs->mana    = f->maxMana;
    fs->meter   = f->maxMeter;
    fs->action  = f->actions[0];
    fs->health = f->maxHealth;       

    fs->position.x = 0;
    Fighter_StartActionIndex(f, 15, -1);
}

void UpdateHitboxes(){
    Action*    lastAct = cb_last(asamiya_f->stateHistory).action;
    Animation* lastAni = cb_last(asamiya_f->stateHistory).animation;

    lastAct->currentFrame += (!currentMatch.paused) + IsKeyPressed(KEY_PERIOD) - IsKeyPressed(KEY_COMMA);
    lastAni->currentFrame +=                          IsKeyPressed(KEY_PERIOD) - IsKeyPressed(KEY_COMMA);

    if (!currentMatch.paused){
        lastAct->currentFrame++;
        if (lastAct->currentFrame == lastAct->mustLinkAfter){
            Fighter_StartAction(asamiya_f, lastAct, -1);
            lastAct = cb_last(asamiya_f->stateHistory).action;
            lastAni = cb_last(asamiya_f->stateHistory).animation;
        }
    }

    for(int i = 0; i < sb_count(lastAct->hitboxes); i++){
        lastAct->hitboxes[i]->currentFrame += (!currentMatch.paused) + IsKeyPressed(KEY_PERIOD) - IsKeyPressed(KEY_COMMA);
    }

    const int modMax = (lastAni->frameCount * lastAni->frameWait);

    lastAni->currentFrame = (lastAni->currentFrame + modMax) % modMax;
    lastAct->currentFrame = lastAni->currentFrame;
    for(int i = 0; i < sb_count(lastAct->hitboxes); i++){
        Hitbox* hb = lastAct->hitboxes[i];

        hb->currentFrame = lastAni->currentFrame;
        hb->active = (hb->currentFrame >= hb->activeOnFrame) && (hb->currentFrame < hb->offOnFrame);
    }

    int delta = IsKeyPressed(KEY_RIGHT_BRACKET) - IsKeyPressed(KEY_LEFT_BRACKET);
    if (delta){
        int idx = lastAct->index + delta;
        idx = (idx + sb_count(asamiya_f->actions)) % sb_count(asamiya_f->actions);
        Fighter_StartActionIndex(asamiya_f, idx, -1);
        lastAct->currentFrame = 0;
        lastAni->currentFrame = 0;
    }

    selectedBoxType += IsKeyPressed(KEY_THREE) - IsKeyPressed(KEY_ONE);
    selectedBoxType = (selectedBoxType + boxtype_max) % boxtype_max;
    selectedBoxIdx  += IsKeyPressed(KEY_E) - IsKeyPressed(KEY_Q);

    Hitbox* selectedBox = 0;

    if (selectedBoxType == boxtype_hit && selectedBoxIdx >= 0 && selectedBoxIdx < sb_count(lastAct->hitboxes))
            selectedBox = lastAct->hitboxes[selectedBoxIdx];

    else if (selectedBoxType == boxtype_hurt && selectedBoxIdx >= 0 && selectedBoxIdx < sb_count(lastAct->hurtboxes))
            selectedBox = lastAct->hurtboxes[selectedBoxIdx];

    else if (selectedBoxType == boxtype_shove)
        selectedBox = &lastAct->shovebox;
    
    if (selectedBox){
        selectedBox->rect.x += (IsKeyDown(KEY_D) - IsKeyDown(KEY_A)) * (1 + 9*IsKeyDown(KEY_LEFT_SHIFT));
        selectedBox->rect.y += (IsKeyDown(KEY_S) - IsKeyDown(KEY_W)) * (1 + 9*IsKeyDown(KEY_LEFT_SHIFT));
        selectedBox->rect.w += (IsKeyDown(KEY_KP_6) - IsKeyDown(KEY_KP_4)) * (1 + 9*IsKeyDown(KEY_LEFT_SHIFT));
        selectedBox->rect.h += (IsKeyDown(KEY_KP_5) - IsKeyDown(KEY_KP_8)) * (1 + 9*IsKeyDown(KEY_LEFT_SHIFT));
    }

    // Return: Create new box
    if (IsKeyPressed(KEY_ENTER)) {
        switch (selectedBoxType) {
            case boxtype_hit: {
                selectedBoxIdx = sb_count(lastAct->hitboxes);
                Hitbox* hb = Hitbox_Create((RectI){100, -100, 100, 100}, 0, lastAni->frameCount);
                sb_push(lastAct->hitboxes, hb);
                break;
            }

            case boxtype_hurt: {
                selectedBoxIdx = sb_count(lastAct->hurtboxes);
                Hurtbox* hb = Hurtbox_Create((RectI){100, -100, 100, 100});
                sb_push(lastAct->hurtboxes, hb);
                break;
            }
        }
    }

    // Delete: Nulls current box
    if(IsKeyPressed(KEY_DELETE)){
        switch (selectedBoxType){
            case boxtype_hit:
                if (selectedBoxIdx < sb_count(lastAct->hitboxes)){
                    Hitbox* hb = lastAct->hitboxes[selectedBoxIdx];
                    hb->active = 0;
                    hb->activeOnFrame = 0;
                    hb->offOnFrame = 0;
                    hb->rect = (RectI){0,0,0,0};
                    PruneBoxes(lastAct->hitboxes, boxtype_hit);
                }
                break;

            case boxtype_hurt:
                if (selectedBoxIdx < sb_count(lastAct->hurtboxes)){
                    Hurtbox* hb = lastAct->hurtboxes[selectedBoxIdx];
                    hb->rect = (RectI){0,0,0,0};
                    PruneBoxes(lastAct->hurtboxes, boxtype_hurt);
                }
                break;
            case boxtype_shove:
                lastAct->shovebox.rect = (RectI){0,0,0,0};
        }
    }
}

void main(){

    SetupRaylib();
    SetupData();
    SetupFighter();
    currentMatch.history = NULL;
    currentMatch.history = cb_init(currentMatch.history, 1);
    currentMatch.paused = true;
    cb_push(currentMatch.history, (MatchState) {});

    
    cb_tab_act();

    #define infoTextLen 9
    const char* infoText[infoTextLen] = {
        "Arrows: Pan",
        "Space : Play/Pause",
        "KP +/- : Zoom",

        "1, 3 : Select box type",
        "Q, E : Select box index",

        "WASD : Move selected box",
        "KP 8456: Resize box",

        "Return : Create new box",
        "Delete : Delete box",
    };

    while(!WindowShouldClose()){

        currentMatch.paused = currentMatch.paused ^ IsKeyPressed(KEY_SPACE);

        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)){
            Fighter_Save(asamiya_f, "newchara.json");
        }

        if (IsKeyPressed(KEY_KP_DECIMAL)){
            previewCamera.x = -6000;
            previewCamera.y = -4700;
        }

        fighterDrawScale += 1/60.0f * (IsKeyDown(KEY_KP_ADD) - IsKeyDown(KEY_KP_SUBTRACT));
        fighterDrawScale = fighterDrawScale * !IsKeyDown(KEY_KP_0) + IsKeyDown(KEY_KP_0);

        previewCamera.x += (9 * IsKeyDown(KEY_LEFT_SHIFT) + 1) * (IsKeyDown(KEY_LEFT) - IsKeyDown(KEY_RIGHT));
        previewCamera.y += (9 * IsKeyDown(KEY_LEFT_SHIFT) + 1) * (IsKeyDown(KEY_UP) - IsKeyDown(KEY_DOWN));
        
        if(true){
            UpdateHitboxes();
        }
        else{

        }

        BeginDrawing();
        // printf("Begin\n");
        ClearBackground(BLACK);

        float dir;
        if (dir = GetMouseWheelMove()){
            cb_alist_scroll(-(int)dir);
        }

        Menu_Draw(root, (Vec2I) {0,0});

    for(int i = 0; i < infoTextLen; i++)
        DrawText(infoText[i],
            3 * 1280/4 + 45, 35 + i * 20,
            20, WHITE
        );

        // printf("End\n");
        EndDrawing();
    }


    printf("Goodbye world!\n");
}