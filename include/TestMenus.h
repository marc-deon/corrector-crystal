#ifndef TESTMENUS
#define TESTMENUS
#include "Ui_Base.h"
#include "Ui_Types.h"
#include "Ui_Sprite.h"
#include "SDL2/SDL.h"

UiMenu* MakeMainMenu(SDL_Renderer* gRenderer);
UiMenu* MakeArcadeMenu();
UiMenu* MakeVersusMenu();
UiMenu* MakeTrainingMenu();
UiMenu* MakeReplaysMenu();
UiMenu* MakeOptionsMenu();
extern SDL_Renderer* gRenderer;
void cb_back();

extern SDL_Texture* mainMenuButtonBg;
extern SDL_Texture* mainMenuButtonTextTextures[5];
extern SDL_Rect mainMenuButtonRects[5];
extern SDL_Surface* mainMenuButtonSurfaces[5];
extern SDL_Texture* mainMenuGrillTexture;
extern SDL_Rect mainMenuGrillRect;

#endif /* TESTMENUS */
