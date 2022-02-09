#ifndef RECTI_H
#define RECTI_H

#include "Vec2I.h"
#include "stdbool.h"

// typedef struct {
//     Vec2I pos;
//     Vec2I size;
// } RectI;

typedef union RectI {
    struct {
        Vec2I pos;
        Vec2I size;
    };

    struct {
        int x;
        int y;
        int w;
        int h;
    };
} RectI;

extern RectI NULLRECT;

RectI CreateReactangle(int x, int y, int w, int h);

// Return a copy of the given RectI with x and width negated.
RectI Rect_Flip(RectI rect);

// Return true or false if RectIs overlap.
bool Rect_Overlap(RectI r1, RectI r2);

int Rect_ShortestEscape(RectI rect1, RectI rect2);

#endif