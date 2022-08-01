#include "Ui_List.h"

UiList* List_New(){
    UiList* l = malloc(sizeof(UiList));
    l->position = (Vec2I) {0,0};
    l->id = uiidcount++;
    l->type = UI_TYPE_LIST;
    l->itemSource = NULL;
    // return l;
}

void List_Draw(UiList*list, Vector2 offset){
    int height = 0;
    int y = 0;
    const int FONTSIZE = 20;

    ListViewInfo* lvi = &(list->data);

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