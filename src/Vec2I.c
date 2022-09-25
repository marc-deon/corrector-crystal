#include "Vec2I.h"


Vec2I Vec2_Add(Vec2I a, Vec2I b) {
    return (Vec2I) {a.x + b.x, a.y + b.y};
}

Vec4I Vec4_Add(Vec4I a, Vec4I b) {
    return (Vec4I) {a.l + b.l, a.r + b.r, a.t + b.t, a.b + b.b};
}