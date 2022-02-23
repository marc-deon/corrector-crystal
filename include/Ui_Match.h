#ifndef UI_MATCH_H
#define UI_MATCH_H

#include "Fighter.h"
#include "Match.h"
#include "Ui_Base.h"

typedef Match Ui_Match;

void Ui_Match_MakeColors();
bool Ui_Match_MakeFonts();
bool Ui_Match_MakeNames(char* p1Name, char* p2Name);

bool Ui_Match_Init(Match* m);

void DrawHUD(Match* m);
void DrawHUD_Training(Match* m);
#endif