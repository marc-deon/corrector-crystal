#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "Stick.h"
#include "Vec2I.h"
#include "CC_Consts.h"
#include "Fighter.h"
#include <assert.h>

#define USE_COLOR true
#define START_RED "\033[1;31m"
#define START_YELLOW "\033[1;33m"
#define START_BLUE "\033[1;34m"
#define END_COLOR "\033[0m"

// Light punch = PUNCH | LIGHT = 0b11000
#define PUNCH  0b10000
#define KICK   0b00000
#define LIGHT  0b01000
#define MEDIUM 0b00100
#define HEAVY  0b00010

bool Stick_IsButtonJustDown(Stick* s, int button){
    int thisFrame = (Stick_StickStateToInt(s->buffer[0]) & button);
    int lastFrame = (Stick_StickStateToInt(s->buffer[1]) & button);
    int ret = thisFrame && !lastFrame;
    return (bool)ret;
}

bool Stick_IsButtonJustUp(Stick* s, int button){
    int thisFrame = (Stick_StickStateToInt(s->buffer[0]) & button);
    int lastFrame = (Stick_StickStateToInt(s->buffer[1]) & button);
    int ret = !thisFrame && lastFrame;
    return (bool)ret;
}

bool Stick_IsButtonDown(Stick* s, int button){
    return Stick_StickStateToInt(s->buffer[0]) & button;
}

// Return an integer version of the given StickState where each button corrosponds to a bit
int Stick_StickStateToInt(StickState ss){
    return
        ss.up       * STICK_UP     |
        ss.down     * STICK_DOWN   |
        ss.left     * STICK_LEFT   |
        ss.right    * STICK_RIGHT  |
        ss.a        * STICK_A      |
        ss.b        * STICK_B      |
        ss.c        * STICK_C      |
        ss.x        * STICK_X      |
        ss.y        * STICK_Y      |
        ss.z        * STICK_Z      |
        ss.start    * STICK_START  |
        ss.select   * STICK_SELECT 
    ;
}

void Stick_PrintState(Stick* st){

    if (Stick_StickStateToInt(st->buffer[0]) == Stick_StickStateToInt(st->buffer[1]))
        return;


    if (USE_COLOR)
        if (st->playerNumber == 1)
            printf(START_RED);
        else
            printf(START_BLUE);

    StickState s = st->buffer[0];



    printf(
" ________/P‾%d\\_________ \n\
|         %d %d          |\n\
|     %d         %d %d %d  |\n\
|   %d   %d      %d %d %d   |\n\
|      %d               |\n\
|______________________|\n",
st->playerNumber, s.select, s.start, s.up, s.x, s.y, s.z, s.left, s.down, s.right, s.a, s.b, s.c
);
    if(USE_COLOR)
        printf(END_COLOR);
}


void Stick_AddState(Stick* stick, StickState state){
    
    for(int i = INPUT_BUFFER_SIZE-1; i > 0; i--){
        stick->buffer[i] = stick->buffer[i-1];
    }
    stick->buffer[0] = state;
}

StickState* Stick_GetState(Stick* stick){
    return &(stick->buffer[0]);
}


// Count the number of tokens in str, where a token is grouped between startGroup and endGroup, or otherwise is a single character.
// e.g. 236A -> 4, 23(6A) -> 3
int Stick_CountTokens(char* str, char* groups){
    int i = 0;
    int count = 0;
    bool pause = false;
    while (str[i] != '\0'){
        
        int j = 0;
        while (groups[j] != '\0'){
            char startGroup = groups[j];
            char endGroup = groups[j+1];

            if(str[i] == startGroup){
                pause = true;
            }

            else if(str[i] == endGroup){
                pause = false;
            }

            j+=2;
        }


        if(!pause){
            count++;
        }
        
        i++;
    }

    return count;
}

// Return a copy of the given StickState with left and right flipped
StickState Stick_FlipState(StickState ss){
    StickState newSs = ss;
    newSs.left = ss.right;
    newSs.right = ss.left;
    return newSs;
}

// Check if the stick state has the given button (or direction combo) activated
// TODO(#13): (Maybe not in this function) account for sideswaps?
int Stick_GetPattern_SingleToken(StickState ss, char token, int punches, int kicks){
    int success = 0;

    switch(token){
        case '1':
            success = ss.down && ss.left;
            break;
        case '2':
            success = ss.down && !ss.left && !ss.right;
            break;
        case '3':
            success = ss.down && ss.right;
            break;
        case '4':
            success = ss.left && !ss.down && !ss.up;
            break;
        case '5':
            success = !(ss.left || ss.right || ss.up || ss.down);
            break;
        case '6':
            success = ss.right && !ss.down && !ss.up;
            break;
        case '7':
            success = ss.up && ss.left;
            break;
        case '8':
            success = ss.up && !ss.left && !ss.right;
            break;
        case '9':
            success = ss.up && ss.right;
            break;
        case 'A':
            success = ss.a;
            break;
        case 'B':
            success = ss.b;
            break;
        case 'C':
            success = ss.c;
            break;
        case 'X':
            success = ss.x;
            break;
        case 'Y':
            success = ss.y;
            break;
        case 'Z':
            success = ss.z;
            
            break;

        // For P and K, we need to be a little more specific with what we return. All non-zero ints are truish, so this
        // is fine for boolean purposes, i.e. when this function is called straight from GetPattern().
        // When it's being called from GetPattern_GroupToken, we're generally expecting a group like (PP).
        // In this case, we don't want the same button to count for both tokens; we need to keep track of which we count.
        // We use a 5 bit number for this: 
        //      The MSB is 1 for punches, 0 for kicks.
        //      The next 3 represent each button.
        //      The LSB is 0 padding to distingush button A from bool true.
        case 'P':

            // punches & LIGHT will be 0 only if we haven't yet processed a light punch (otherwise it will be 0b01000). Similar for others.
            if ((punches & LIGHT) == 0 && ss.x){
                    success = PUNCH | LIGHT;
            }
            else if ((punches & MEDIUM) == 0 && ss.y){
                    success = PUNCH | MEDIUM;
            }
            else if ((punches & HEAVY) == 0 && ss.z){
                    success = PUNCH | HEAVY;
            }
            break;

        case 'K':
            if ((kicks & LIGHT) == 0 && ss.a){
                    success = KICK | LIGHT;
            }
            else if ((kicks & MEDIUM) == 0 && ss.b){
                    success = KICK | MEDIUM;
            }
            else if ((kicks & HEAVY) == 0 && ss.c){
                    success = KICK | HEAVY;
            }
            break;

        // Generic 'Up'; 7, 8, or 9
        case 'u':
            success = ss.up;
            break;

        // Generic 'Middle'; 4, 5, or 6
        case 'm':
            success = !(ss.up || ss.down);
            break;

        // Generic 'Down'; 1, 2, or 3
        case 'd':
            success = ss.down;
            break;

        // Generic 'Left'; 1, 4, or 7
        case 'b':
            success = ss.left;
            break;

        // Generic 'Right'; 3, 6, or 9
        case 'f':
            success = ss.right;
            break;

        default:
            printf("Error! Default caused by character [%c]: ", token);
            assert(0);
            success = 0;
            break;
    }

    return success;
}


// Use this for parenthetically grouped tokens
// returns a Vec2I where x is a 1 or 0 for find success, and y is the number of indices to move forward.
// NOTE: startIndex will be the index of '(', and should be incremented by return.y to index of ')' such that
// the next iteration of the loop will start one character after ')'.
Vec2I Stick_GetPattern_GroupToken(StickState ss, char* pattern, int startIndex, char groupEnd){
    int increment = 1;
    int i = startIndex+1;
    int success = true;
    

    int punches = PUNCH; //0b10000
    int kicks =   KICK;  //0b00000

    // Look for every token in the given stick state
    while(pattern[i] != groupEnd){
        // As soon as there's a failure, mark success as false. Keep incrementing.
        int result = Stick_GetPattern_SingleToken(ss, pattern[i], punches, kicks);
        // Generic P or K
        if(result > 1){
            // P
            if((result & PUNCH) == PUNCH){
                punches = punches | result;
            }
            // K
            else if ((result & KICK) == KICK){
                kicks = kicks | result;
            }
            else{
                printf("\n\n\n\nError! GetPattern_GroupToken: Unknown result %d: __FILE__(__LINE__) \n\n\n\n", result);
            }
        }

        if(result == 0){
            success = false;
        }

        increment++;
        i++;
    }

    Vec2I ret = {.x = success, .y = increment};
    return ret;
}

Vec2I Stick_GetPattern_ChargeToken(Player* p, Stick* stick, Motion* motion, int charStartIndex, int frameStartIndex, char groupEnd){
    int success = false;
    char* pattern = motion->pattern;

    int charEndIndex = charStartIndex;

    while (pattern[charEndIndex] != groupEnd) {
        charEndIndex++;
    }
  
    char copy[charEndIndex-charStartIndex + 1];

    // Make a backup, because splitting occurs in-place
    TextCopy(copy, pattern+1);

    // First part is the buttons, second part is the frames.
    char* butts = strtok(copy, "-");
    char* frames = strtok(NULL, "-");
    int length = atoi(frames);

    int consecutive = 0;
    // Starting at the most recent state, go backwards and see if our match holds
    // for(int curFrame = startFrame; curFrame >= endFrame; curFrame--){
    int endFrame = 0;
    for(int curFrame = frameStartIndex; curFrame >= endFrame; curFrame--){

        // Some motions have stricter timing than the buffer
        if(curFrame > motion->bufferSize)
            continue;

        uint match = Stick_GetPattern(p, stick, motion, butts, curFrame);

        // Reset to 0 if no match
        consecutive++;
        consecutive *= match;

        if (consecutive == length){
            printf("succ break\n", consecutive);
            success = true;
            break;
        }
    }

    Vec2I ret = {.x = success, .y = charEndIndex-charStartIndex};
    return ret;
}



/*
Note: These patterns do not determine if the action is aerial or grounded. There are no prefices for j., c., f., etc.
Jumping moves are instead determined by specifying "airOk" as true.
There are no close or far actions because those are wack.

The Format of these patterns is a little different from regular numpad notation.
Numbers are used for directions, And ABC-XYZ are used for buttons.
Additionally, P may be used to represent any XYZ, and K any ABC.
This allows, for example, pushblock to be any P+P instead of needing to specify.

Charge moves are notated with the regular square brackets, with the number of frames specifed after a hyphen,
e.g. [4-30]6Z is a sonic boom motion that requires 30 frames of charging.
e.g. [2-30]8C flash kick

Standard notation does nothing to specify if two buttons need to be pressed on the same frame, i.e. 6C vs 236C.
The expectation is that 6C requires both buttons be pressed on the same frame (it's a command normal),
while 236C allows for (but does not *require*!) a small pause between 6 and C (it's a motion).

Stanard notation is fine for humans, who can fill in that gap with an assumption. It's not enough for computers, however.
This notation makes it more explicit by grouping same-frame inputs with parentheses. So for example:
    A common fireball is notated as 236X.
    A common overhead command normal is notated as (6Z).
    A common launcher is notated as (3C).
    Talbain super is XY6AB (regularly notated as LP MP 6 LK MK).
    Common 6 button grabs are notated as (AX) (regularly LP+LK), (PP) (regularly P+P), or (6Z) (regularly 6HP).

This is all hopefully equally alienting to both anime and non-anime players alike.
However, I think it skews slightly too much towards anime notation. So there's one more thing to add:
u,d,f,b (lowercase) can be used for up, down, forward, back. m can also be used for 'middle', which is 4/5/6.
While these will work to replace number notation, e.x. dfZ instead of 3Z, that is not their intended purpose.
Their purpose is to reduce redundancy in movement, e.x. so that all standing->crouching and crouching->standing
actions can be covered by md and dm respectively.

*/ 
bool Stick_GetPattern(struct player* p, Stick* stick, Motion* motion, char* patt_override, int frame){
    char* pattern = motion->pattern;
    // For charge moves
    if (patt_override){
        pattern = patt_override;
    }
    
    int len = Stick_CountTokens(pattern, "()[]");

    int matchesLeft = len;
    int patternIndex = 0;


    int startFrame = INPUT_BUFFER_SIZE-1;
    int endFrame = 0;

    if(frame >= 0){
        startFrame = frame;
        endFrame = frame;
    }

    // The stick buffer is a queue that we want to search oldest to newest
    for(int curFrame = startFrame; curFrame >= endFrame; curFrame--){

        // Some motions have stricter timing than the buffer
        if(curFrame > motion->bufferSize)
            continue;

        char currentCharacter = pattern[patternIndex];
        StickState currentState = stick->buffer[curFrame];
        if(!Fighter_FacingRight(p->pointCharacter)){
            currentState = Stick_FlipState(currentState);
        }
        // if the current character was matched
        bool match = false;
        
        // Group token
        if (currentCharacter == '('){
            Vec2I v = Stick_GetPattern_GroupToken(currentState, pattern, patternIndex, ')');
            
            match = v.x;
            if(match){
                patternIndex += v.y - 1;
            }
        }

        // Charge token
        else if (currentCharacter == '['){
            Vec2I v = Stick_GetPattern_ChargeToken(p, stick, motion, patternIndex, curFrame, ']');
            match = v.x;
            if (match){
                patternIndex += v.y - 1;
            }
        }
        
        else if (currentCharacter == ')'){
            patternIndex++;
        }


        else if (currentCharacter == ']'){
            patternIndex++;
        }

        // Single character token
        else {
            match = Stick_GetPattern_SingleToken(currentState, currentCharacter, PUNCH, KICK);
        }

        if(match){
            matchesLeft--;
            patternIndex++;
            
        } 
        
        if(matchesLeft == 0){
            
            return true;
        }
    }
    return false;
}
