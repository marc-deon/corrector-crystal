#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Vector2.h"
#include "stdbool.h"

typedef struct rectangle{
    Vector2 pos;
    Vector2 size;
} Rectangle;

extern Rectangle NULLRECT;

Rectangle CreateReactangle(int x, int y, int w, int h);

// Return a copy of the given rectangle with x and width negated.
Rectangle Rect_Flip(Rectangle rect);

// Return true or false if rectangles overlap.
bool Rect_Overlap(Rectangle r1, Rectangle r2);

int Rect_ShortestEscape(Rectangle rect1, Rectangle rect2);

#endif