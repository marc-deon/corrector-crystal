#include "Match.h"
#include "Player.h"
#include "stretchy_buffer.h"
#include "circular_buffer.h"

#define SECONDS_PER_ROUND 60
#define FRAMES_PER_SECOND 60
#define MAX_ROUNDS 2

Match Match_Init(){
    Match m;
    m.type = UI_TYPE_MATCH;
    m.paused = false;
    
    m.maxTime = FRAMES_PER_SECOND * SECONDS_PER_ROUND;
    m.maxRounds = MAX_ROUNDS;
    m.currentRounds[0] = m.currentRounds[1] = 0;
    
    m.history = NULL;

    m.history = cb_init(m.history, MAX_REWIND);
    cb_push(m.history, (MatchState){});
    /*Expanded*/

    // stb__cbinitf(m.history, MAX_REWIND, sizeof(*(m.history)));
    // m.history = &(m.history)[((long int *) (void *) (m.history) - 3)[1]-(MAX_REWIND)];
    
    /*        */

    cb_last(m.history).currentTime = m.maxTime; 
    cb_last(m.history).hitStop = 0;
    return m;
}

void Match_End(Match* m){
    cb_free(m->history);
    Fighter_Destroy(m->players[0].pointCharacter);
    Fighter_Destroy(m->players[1].pointCharacter);
}

// FIXME: This will have errors on rollback, make sure it's interuptable.
// 0 is no win, 1 and 2 are players, 3 is a double KO or tied time out.
int Match_CheckRoundWin(Match* m){
    int i = 0;
    int p1hp = cb_last(m->players[0].pointCharacter->stateHistory).health;
    int p2hp = cb_last(m->players[1].pointCharacter->stateHistory).health;


    if(cb_last(m->history).currentTime == 0){
        i += (p2hp >= p1hp);
        i += (p2hp >= p1hp)*2;
    }
    else{
        // P1 wins if P2's health is 0
        i += (p2hp == 0);
        // P2 wins if P1's health is 0
        i += (p1hp == 0) * 2;
    }

    return i;
}

// 0 is no win, 1 and 2 are players, 3 is tie
int Match_CheckMatchWin(Match* m){
    int i = 0;
    i += m->currentRounds[0] == m->maxRounds;
    i += m->currentRounds[1] == m->maxRounds * 2;
    return i;
}