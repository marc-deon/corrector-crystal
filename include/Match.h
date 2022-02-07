#ifndef MATCH_H
#define MATCH_H
#include "Player.h"
#include "CC_Consts.h"

typedef struct matchState{
    unsigned int currentTime;
    unsigned int hitStop;
} MatchState;

#include "Ui_Base.h"

typedef struct match{
    ui_preamble

    unsigned int maxTime;
    unsigned int maxRounds;
    MatchState* history;
    unsigned int currentRounds[2];
    Player players[2];
    bool paused;

} Match;

Match Match_Init();
void Match_End(Match* m);

int Match_CheckRoundWin(Match* m);
int Match_CheckMatchWin(Match* m);

#endif