#ifndef STICK_H
#define STICK_H
#include <stdbool.h>
#include "Motion.h"
// #include "Player.h"

#define STICK_UP     (1 << 0x0)
#define STICK_DOWN   (1 << 0x1)
#define STICK_LEFT   (1 << 0x2)
#define STICK_RIGHT  (1 << 0x3)
#define STICK_A      (1 << 0x4)
#define STICK_B      (1 << 0x5)
#define STICK_C      (1 << 0x6)
#define STICK_X      (1 << 0x7)
#define STICK_Y      (1 << 0x8)
#define STICK_Z      (1 << 0x9)
#define STICK_START  (1 << 0xa)
#define STICK_SELECT (1 << 0xb)
#define STICK_M1     (1 << 0xc)
#define STICK_M2     (1 << 0xd)
#define STICK_M3     (1 << 0xe)
#define STICK_M4     (1 << 0xf)



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