#ifndef UI_LIST_H
#define UI_LIST_H

#include "Ui_Base.h"

typedef struct {
    int itemCount;
    int focusIndex;  // Which item is currently hovered
    int scrollIndex; // Which item to begin drawing at
    int activeIndex; // Which item is currently active
    int maxHeight;
    char** items;

} ListViewInfo;


typedef struct {
    ui_preamble

    ListViewInfo data;

    void* itemSource;

} UiList;

UiList* List_New();

#endif
