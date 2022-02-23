#define ASAMIYA

#include <raylib.h>

#include <stdio.h>
#include "stretchy_buffer.h"
#include "Match.h"
#include "Fighter.h"
#include "Ui_Types.h"

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

typedef struct {
    ui_preamble
} UiPreview;

UiList* List_New(){
    UiList* l = malloc(sizeof(UiList));
    l->type = UI_TYPE_LIST;
    l->itemSource = NULL;
}

UiPreview* Preview_New(){
    UiPreview* p = malloc(sizeof(UiPreview));
    p->type = UI_TYPE_PREVIEW;
    p->position = (Vec2I) {6969, 6969};
    return p;
}

////////////////////

Fighter* asamiya_f;

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
        printf("[%s]\n", shoppingList.items[i]);
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



UiElement* FindWidgetByPosition(Vector2 pos){
    UiElement* ret = NULL;

    // for (widget in allwidgets){
    //     if (pos in box(widget.position, widget.size)){
    //         ret = widget;
    //         break
    //     }
    // }

    return ret;
}

void SetupRaylib(){
    // SetTraceLogLevel(LOG_DEBUG);
    SetTargetFPS(1);
    InitWindow(1280, 720, "Asamiya Character Creater");
}

void SetupData(){
    // root;
        UiMenu* leftPane;
            UiMenu* leftTabs;
                UiButton* buttAct;
                UiButton* buttAnim;
            UiList* aList;
        UiPreview* preview;
        UiMenu* rightPane;
            UiMenu* rightTabs;
                UiButton* buttInspector;
                UiButton* buttHitboxes;
            UiList* rList;


    root          = Menu_New(BAKE_TYPE_SET_X_SIZE);
    
    leftPane      = Menu_New(BAKE_TYPE_SET_Y_SPACING);
    leftTabs      = Menu_New(BAKE_TYPE_SET_X_SIZE);
    buttAct       = Button_New(leftTabs, "Act", 20);
    buttAnim      = Button_New(leftTabs, "Anim", 20);
    aList         = List_New();
    preview       = Preview_New();
    rightPane     = Menu_New(BAKE_TYPE_SET_Y_SPACING);
    rightTabs     = Menu_New(BAKE_TYPE_SET_X_SIZE);
    buttInspector = Button_New(rightTabs, "Inspector", 20);
    buttHitboxes  = Button_New(rightTabs, "Hitboxes", 20);
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

    // borders, borderoffset, type, centerall
    Menu_Bake(root,
        (Vec4I) {0, 0, 1280, 720},
        (Vec4I) {0,0,0,0},
        BAKE_TYPE_SET_X_SIZE,
        false
    );

    printf("root rect %d %d %d %d\n", mqui_rect(root));
    // exit(0);
}

void main(){

    SetupRaylib();
    SetupData();

    asamiya_f = Fighter_Create("fighterData/superman.jsonc");
    cb_tab_act();

    UiMenu* buttlist = Menu_New(BAKE_TYPE_FLOATING);

    Button_New(buttlist, "Actions", -1);
    Button_New(buttlist, "Animations", -1);

    shoppingList.maxHeight = 720 - 10 - 5 - UiElement_Size(mqui_as_element(buttlist)).y;

    Menu_Bake(buttlist, (Vec4I){.b = 5}, (Vec4I){.l=5, .t=5, .r=0, .b=0}, BAKE_TYPE_SET_X_SPACING, false);

    while(!WindowShouldClose()){
        BeginDrawing();
        printf("Begin\n");
        ClearBackground(BLACK);

        float dir;
        if (dir = GetMouseWheelMove()){
            cb_alist_scroll(-(int)dir);
        }

        Menu_Draw(root);

        // Menu_Draw(buttlist);
        // int buttheight = UiElement_Size(mqui_as_element(buttlist)).y+5;
        // List_Draw(&shoppingList, (Vector2){5, buttheight});
        printf("End\n");
        EndDrawing();
    }


    printf("Goodbye world!\n");
}