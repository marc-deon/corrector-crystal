#ifndef TESTMENUS_H
#define TESTMENUS_H
#include "Ui_Base.h"
#include "Ui_Types.h"
#include "Ui_Sprite.h"

UiMenu* MakeMainMenu();
UiMenu* MakeArcadeMenu();
UiMenu* MakeVersusMenu();
UiMenu* MakeTrainingMenu();
UiMenu* MakeReplaysMenu();
UiMenu* MakeOptionsMenu();
void cb_back();

extern Texture mainMenuButtonBg;
extern Texture mainMenuButtonTextTextures[5];
extern RectI mainMenuButtonRects[5];
extern Texture mainMenuButtonSurfaces[5];
extern Texture mainMenuGrillTexture;
extern RectI mainMenuGrillRect;

#endif /* TESTMENUS */
