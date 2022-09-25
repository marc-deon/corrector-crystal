// #define ASAMIYA
int asamiya = 1;

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
#include "Ui_List.h"

#define MOUSEWHEEL_UP (1f)
#define MOUSEWHEEL_DOWN (-1f)

// ListViewInfo shoppingList = {
//     .itemCount   = -1,
//     .focusIndex  = -1,
//     .scrollIndex = -1,
//     .activeIndex = -1,
//     .maxHeight   = -1,
//     .items = NULL
// };



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
            if (((Hitbox*)(sb[i]))->rect.w == 0 && ((Hitbox*)(sb[i]))->rect.h == 0) {
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
    while(sb_count(sb) > 0){
        int count = sb_count(sb);
        Hitbox* hb = sb_last((Hitbox**)sb);
        if (sb_last((Hitbox**)sb)->rect.w == 0 && sb_last((Hitbox**)sb)->rect.h == 0) {
            switch(t) {
                case boxtype_hit:
                    Hitbox* e1 = sb_pop((Hitbox**)sb);
                    free(e1);
                    break;
                case boxtype_hurt:
                    Hurtbox* e2 = sb_pop((Hurtbox**)sb);
                    free(e2);
                    break;
            }
        }
        else {
            break;
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

void InspectAnimation(){
    
}

void InspectAction(){

}

void cb_tab_act(){

    // UiList* list = mqui_as_menu(root->elements[0])->elements[1];
    // ListViewInfo* = &(list->data);

    // // Free data if already present
    // if(listData->items){
    //     for(int i = 0; i < listData->itemCount; i++){
    //         free(listData->items[i]);
    //     }
    //     free(listData->items);
    // }

    // // Generate list info based on fighter actions
    // listData->itemCount = sb_count(asamiya_f->actions);
    // listData->focusIndex = 0;
    // listData->scrollIndex = 0;
    // listData->activeIndex = 0;


    // listData->items = (char**) malloc(listData->itemCount * sizeof(char*));

    // for (size_t i = 0; i < listData->itemCount; i++)
    // {
    //     listData->items[i] = (char*) malloc(TextLength(asamiya_f->actions[i]->name ) + 1);
    //     TextCopy(listData->items[i], asamiya_f->actions[i]->name);
    // }
    
    
    // // Disable hitboxes button


}

void cb_tab_anim(){

}

void cb_alist_click(){

}

void cb_alist_scroll(int dir){
    // printf("scroll\n");
    // shoppingList.activeIndex = max(0, min(shoppingList.itemCount-1, shoppingList.activeIndex + dir));

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
    cb_push(f->stateHistory, (FighterState){});
    cb_push(f->entity->history, *EntityState_Create());

    Fighter_GetPalette(f, 0);
    
    FighterState* fs = &cb_last(f->stateHistory);
    EntityState* es = &cb_last(f->entity->history);

    fs->stateFlags              = 0;
    fs->tempGravity.x           = 6969;
    fs->tempGravity.y           = 6969;
    es->subframe                = 0;
    es->currentAnimation = f->animations[0];
    es->currentAnimation->currentFrame = 0;

    fs->jumps   = f->maxJumps;
    es->currentHealth  = f->maxHealth;
    fs->mana    = f->maxMana;
    fs->meter   = f->maxMeter;
    es->currentAction  = f->actions[0];

    es->position.x = 0;
    Fighter_StartActionIndex(f, 15, -1);
}

void UpdateHitboxes(){
    Action*    lastAct = cb_last(asamiya_f->entity->history).currentAction;
    Animation* lastAni = cb_last(asamiya_f->entity->history).currentAnimation;

    lastAct->currentFrame += (!currentMatch.paused) + IsKeyPressed(KEY_PERIOD) - IsKeyPressed(KEY_COMMA);
    lastAni->currentFrame +=                          IsKeyPressed(KEY_PERIOD) - IsKeyPressed(KEY_COMMA);

    if (!currentMatch.paused){
        lastAct->currentFrame++;
        if (lastAct->currentFrame == lastAct->mustLinkAfter){
            Fighter_StartAction(asamiya_f, lastAct, -1);
            lastAct = cb_last(asamiya_f->entity->history).currentAction;
            lastAni = cb_last(asamiya_f->entity->history).currentAnimation;
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

    // bound selectedBoxIdx to range [-1, numBoxes-1]
    selectedBoxIdx = max(selectedBoxIdx, -1);
    switch (selectedBoxType) {
        case boxtype_hit:
            selectedBoxIdx = min(selectedBoxIdx, sb_count(lastAct->hitboxes) - 1);
            break;
        
        case boxtype_hurt:
            selectedBoxIdx = min(selectedBoxIdx, sb_count(lastAct->hurtboxes) - 1);
            break;
    }
    

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
                Hitbox* hb = Hitbox_Create((RectI){100, -100, 100, 100}, 0, lastAni->frameCount * lastAni->frameWait);
                // hb->active = lastAni->currentFrame >= hb->activeOnFrame && lastAni < hb->offOnFrame;
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
    if(IsKeyPressed(KEY_DELETE) && selectedBoxIdx >= 0){

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

        "Ctrl+S : Save to json",
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