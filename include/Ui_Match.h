#ifndef UI_MATCH_H
#define UI_MATCH_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Fighter.h"
#include "Match.h"
#include "Ui_Base.h"

typedef Match Ui_Match;

void Ui_Match_MakeColors();
bool Ui_Match_MakeFonts();
bool Ui_Match_MakeNames(char* p1Name, char* p2Name);

extern TTF_Font* nameFont;

bool Ui_Match_Init(Match* m);

void DrawHUD(Match* m);
void DrawHUD_Training(Match* m);
#endif