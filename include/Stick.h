#ifndef STICK_H
#define STICK_H
#include <stdbool.h>
#include "Motion.h"
// #include "Player.h"

#define STICK_UP     0x1
#define STICK_DOWN   0x2
#define STICK_LEFT   0x4
#define STICK_RIGHT  0x8
#define STICK_A      0x10
#define STICK_B      0x20
#define STICK_C      0x40
#define STICK_X      0x80
#define STICK_Y      0x100
#define STICK_Z      0x200
#define STICK_START  0x400
#define STICK_SELECT 0x800


////////////////////////////////////
// Structs for virtual controller //
////////////////////////////////////

struct player;

typedef struct stickState {

/////////////////////////
//       SE ST         //
//    U         X Y Z  //
//  L D R      A B C   //
//                     //
/////////////////////////


    bool up;
    bool down;
    bool left;
    bool right; // 4
    bool a;
    bool b;
    bool c;     // 7
    bool x;
    bool y;
    bool z;     // 10
    bool start;  
    bool select;// 12
    // bool m1; //     Potential macros.
    // bool m2; //     Potential macros.
    // bool m3; //     Potential macros.
    // bool m4; // 16  Potential macros.

} StickState;

#define INPUT_BUFFER_SIZE 20
typedef struct stick {
    int playerNumber; 
    StickState buffer[INPUT_BUFFER_SIZE];

} Stick;


// Print a graphical representation of the stick to std out.
void Stick_PrintState(Stick* st);
void Stick_AddState(Stick* stick, StickState state);
StickState* Stick_GetState(Stick* stick);

// Return true if the provided pattern is matched; to look for multiple patterns, see the relevent Player function.
bool Stick_GetPattern(struct player* p, Stick* stick, Motion* motion, char* patt, int frame);
bool Stick_IsButtonJustDown(Stick* s, int button);
bool Stick_IsButtonDown(Stick* s, int button);
bool Stick_IsButtonJustUp(Stick* s, int button);
bool Stick_IsButtonUp(Stick* s, int button);

int Stick_StickStateToInt(StickState ss);

#endif