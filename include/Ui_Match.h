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
bool Ui_Match_MakeNames(SDL_Renderer* ren, char* p1Name, char* p2Name);

extern TTF_Font* nameFont;
extern SDL_Color WHITE;

bool Ui_Match_Init(SDL_Renderer* ren, Match* m);

void DrawHUD(SDL_Renderer* ren, Match* m);
void DrawHUD_Training(SDL_Renderer* ren, Match* m);
#endif