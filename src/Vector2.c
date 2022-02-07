#ifndef VECTOR2
#define VECTOR2

#include "Vector2.h"


Vector2 Vec2_Add(Vector2 a, Vector2 b){
    return (Vector2) {a.x + b.x, a.y + b.y};
}

Vector4 Vec4_Add(Vector4 a, Vector4 b){
    return (Vector4) {a.l + b.l, a.r + b.r, a.t + b.t, a.b + b.b};
}

#endif /* VECTOR2 */
