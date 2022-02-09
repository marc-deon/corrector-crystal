#include "Vec2I.h"
#include "RectI.h"

RectI NULLRECT = {0,0,0,0};

RectI CreateReactangle(int x, int y, int w, int h){
    RectI r;
    r.pos.x = x;
    r.pos.y = y;
    r.size.x = w;
    r.size.y = h;
    return r;
}

RectI Rect_Flip(RectI rect){
    RectI nr;
    nr.pos.x = -rect.pos.x;
    nr.pos.y = rect.pos.y;
    nr.size.x = -rect.size.x;
    nr.size.y = rect.size.y;
    return nr;
}

// Return the lesser of two inputs
#define min(a,b) (a < b ? a : b)

// Return the greater of two inputs
#define max(a,b) (a > b ? a : b)

bool Rect_Overlap(RectI rect1, RectI rect2){
    // This math assumes [left x, top y, right x, bottom y],
    // so create these.
    int r1[4] = {
        min(rect1.pos.x, rect1.pos.x + rect1.size.x),
        min(rect1.pos.y, rect1.pos.y + rect1.size.y),
        max(rect1.pos.x, rect1.pos.x + rect1.size.x),
        max(rect1.pos.y, rect1.pos.y + rect1.size.y),
    };
    int r2[4] = {
        min(rect2.pos.x, rect2.pos.x + rect2.size.x),
        min(rect2.pos.y, rect2.pos.y + rect2.size.y),
        max(rect2.pos.x, rect2.pos.x + rect2.size.x),
        max(rect2.pos.y, rect2.pos.y + rect2.size.y),
    };


    return !(
        r1[2] <= r2[0] || // extents of r1 are left of r2
        r1[3] <= r2[1] || // extents of r1 are above r2
        r1[0] >= r2[2] || // extents of r1 are right of r2
        r1[1] >= r2[3]    // extents of r1 are below r2
    );
}

// Return the x direction in which rect1 should move to reduce the area of overlap with rect2.
// 1 for right, -1 for left. If RectIs don't overlap, return 0.
// TODO(#12): This *technically* has a built in bias towards moving left, which
// could result in unwanted behavior if both RectIs overlap perfectly.
// It's very, very, very unlikely that this would ever be the case, though.
int Rect_ShortestEscape(RectI rect1, RectI rect2){
    
    if(!Rect_Overlap(rect1, rect2)){
        return 0;
    }

    int r1[4] = {
        min(rect1.pos.x, rect1.pos.x + rect1.size.x),
        min(rect1.pos.y, rect1.pos.y + rect1.size.y),
        max(rect1.pos.x, rect1.pos.x + rect1.size.x),
        max(rect1.pos.y, rect1.pos.y + rect1.size.y),
    };
    int r2[4] = {
        min(rect2.pos.x, rect2.pos.x + rect2.size.x),
        min(rect2.pos.y, rect2.pos.y + rect2.size.y),
        max(rect2.pos.x, rect2.pos.x + rect2.size.x),
        max(rect2.pos.y, rect2.pos.y + rect2.size.y),
    };

    int currentArea = 
        (min(r1[2], r2[2]) - max(r1[0], r2[0])) *
        (max(r1[3], r2[3]) - min(r1[1], r2[1]));
    
    if (!currentArea)
        return 0;

    // Try moving rect1 to the right by a bit
    int rightArea =
        (min(r1[2] + 1, r2[2]) - max(r1[0] + 1, r2[0])) *
        (max(r1[3], r2[3]) - min(r1[1], r2[1]));

    // Try moving rect1 to the left by a bit
    int leftArea = 0;
        (min(r1[2] - 1, r2[2]) - max(r1[0] - 1, r2[0])) *
        (max(r1[3], r2[3]) - min(r1[1], r2[1]));

    if(rightArea < leftArea)
        return 1;
    return -1;

}
