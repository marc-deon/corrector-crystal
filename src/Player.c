#include "Player.h"
#include "stretchy_buffer.h"
#include "circular_buffer.h"

// Find the first pattern in the stick buffer supported by the point character and return its index. Returns -1 if not found.
int Player_GetFirstPattern(Player* p){

    for(int i = 0; i < sb_count(p->pointCharacter->motions); i++){
        Motion* motion = p->pointCharacter->motions[i];
        
        char* pattern = motion->pattern;

        if (motion == 0 || pattern == 0){
            continue;
        }

        Action* curAct = cb_last(p->pointCharacter->entity->history).currentAction;
        if (motion->action->linksFrom != NULL && motion->action->linksFrom != curAct){
            continue;
        }

        if (cb_last(p->pointCharacter->stateHistory).frame < motion->action->canLinkAfter){
            continue;
        }

        if(
            // Skip checking the pattern if the priority is less than or equal to our current action priority
            (motion->priority <= cb_last(p->pointCharacter->entity->history).currentAction->priority)
            // BUT make a special exception if the priorities are both equal to 1, for walking.
            // Without this, if walking is pri 1, then plinking 64 in two frames (without hitting 5)
            // Will cause a moonwalk. 46 will cause a reverse-moonwalk.
            && !((motion->priority == 1) && cb_last(p->pointCharacter->entity->history).currentAction->priority == 1)
            ){
            continue;
        }


        if (cb_last(p->pointCharacter->stateHistory).jumps < motion->action->burnsJump){
            continue;
        }
    
        // If air and air, or ground and ground...
        if( (motion->airOk && !Fighter_Grounded(p->pointCharacter)) || (!motion->airOk && Fighter_Grounded(p->pointCharacter))){
            if(Stick_GetPattern(p, p->stick, motion, NULL, -1)){
                return i;
            }
        }
    }

    return -1;
}
