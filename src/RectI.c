#include "Vec2I.h"
#include "RectI.h"

RectI NULLRECT = {0,0,0,0};

RectI CreateReactangle(int x, int y, int w, int h) {
    return (RectI) {x,y,w,h};
}

RectI Rect_Flip(RectI rect) {
    RectI nr = rect;
    nr.x  *= -1;
    nr.w *= -1;
    return nr;
}

RectI Rect_Flip_Draw(RectI rect) {
    RectI nr = rect;
    nr.x  = - rect.x - rect.w;
    return nr;
}

bool Rect_Overlap(RectI rect1, RectI rect2) {
    // This math assumes [left x, top y, right x, bottom y],
    // so create these.
    int r1[4] = {
        min(rect1.x, rect1.x + rect1.w),
        min(rect1.y, rect1.y + rect1.h),
        max(rect1.x, rect1.x + rect1.w),
        max(rect1.y, rect1.y + rect1.h),
    };
    int r2[4] = {
        min(rect2.x, rect2.x + rect2.w),
        min(rect2.y, rect2.y + rect2.h),
        max(rect2.x, rect2.x + rect2.w),
        max(rect2.y, rect2.y + rect2.h),
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
int Rect_ShortestEscape(RectI rect1, RectI rect2) {
    
    if(!Rect_Overlap(rect1, rect2)) {
        return 0;
    }

    int r1[4] = {
        min(rect1.x, rect1.x + rect1.w),
        min(rect1.y, rect1.y + rect1.h),
        max(rect1.x, rect1.x + rect1.w),
        max(rect1.y, rect1.y + rect1.h),
    };
    int r2[4] = {
        min(rect2.x, rect2.x + rect2.w),
        min(rect2.y, rect2.y + rect2.h),
        max(rect2.x, rect2.x + rect2.w),
        max(rect2.y, rect2.y + rect2.h),
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
    // TODO: ... what the fuck am i doing here
    int leftArea = 0;
        (min(r1[2] - 1, r2[2]) - max(r1[0] - 1, r2[0])) *
        (max(r1[3], r2[3]) - min(r1[1], r2[1]));

    if(rightArea < leftArea)
        return 1;
    return -1;

}
