
#include "Ui_Types.h"
#include "Ui_Preview.h"
#include <stdio.h>
#include "stretchy_buffer.h"
#include "circular_buffer.h"
#include "Fighter.h"

Fighter* asamiya_f;
RectI previewCamera;

UiPreview* Preview_New(){
    UiPreview* p = malloc(sizeof(UiPreview));
    p->position = (Vec2I) {0,0};
    p->id = uiidcount++;
    p->type = UI_TYPE_PREVIEW;
    p->position = (Vec2I) {6969, 6969};

    previewCamera.x = -6000;
    previewCamera.y = -4700;
    previewCamera.w = ui_w(p);
    previewCamera.h = ui_h(p);

    fighterDrawScale = 2;

    return p;
}

const char* boxtypes[] = {
    "None",
    "Hit",
    "Hurt",
    "Shove",
};

void Preview_Draw(UiPreview* p, Vec2I offset){
    Action* act = cb_last(asamiya_f->stateHistory).action;
    Animation* ani = cb_last(asamiya_f->stateHistory).animation;
    int y = 2;

    DrawRectangle(p->position.x + offset.x, p->position.y + offset.y, ui_w(p), ui_h(p), PURPLE);


    const char* text = TextFormat("Camera: %d %d", previewCamera.x, previewCamera.y);
    DrawText(text, p->position.x + offset.x + 2, p->position.y + offset.y + y, 20, WHITE); y += 20;

    text = TextFormat("(%d) %s", act->index, act->name);
    DrawText(text, p->position.x + offset.x + 2, p->position.y + offset.y + y, 20, WHITE); y += 20;

    text = TextFormat("f %d/%d", ani->currentFrame, ani->frameCount * ani->frameWait - 1);
    DrawText(text, p->position.x + offset.x + 2, p->position.y + offset.y + y, 20, WHITE); y += 20;

    text = TextFormat("Selected type: %s", boxtypes[selectedBoxType]);
    DrawText(text, p->position.x + offset.x + 2, p->position.y + offset.y + y, 20, WHITE); y += 20;

    text = TextFormat("Box index: %d", selectedBoxIdx);
    DrawText(text, p->position.x + offset.x + 2, p->position.y + offset.y + y, 20, WHITE); y += 20;

    text = TextFormat("hb count: %d", sb_count(act->hitboxes));
    DrawText(text, p->position.x + offset.x + 2, p->position.y + offset.y + y, 20, WHITE); y += 20;

    Fighter_DrawSprite(asamiya_f, previewCamera);
    Fighter_DrawCollisionbox(asamiya_f, previewCamera);
    Fighter_DrawHitboxes(asamiya_f, previewCamera);
    Fighter_DrawHurtboxes(asamiya_f, previewCamera);
    Fighter_DrawPoint(asamiya_f, previewCamera);

    printf("Fighter drawn\n");

}