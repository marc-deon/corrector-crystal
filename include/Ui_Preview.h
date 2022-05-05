#ifndef UI_PREVIEW_H
#define UI_PREVIEW_H

#include "Ui_Base.h"
#include "Fighter.h"

extern Fighter* asamiya_f;
extern RectI previewCamera;


typedef struct {
    ui_preamble
} UiPreview;

UiPreview* Preview_New();
void Preview_Draw(UiPreview* p, Vec2I offset);

#endif