#ifndef VEC2I_H
#define VEC2I_H

typedef struct vec2I {
    int x;
    int y;
} Vec2I;

typedef struct vec4I {
    int l;
    int r;
    int t;
    int b;
} Vec4I;

Vec2I Vec2_Add(Vec2I a, Vec2I b);
Vec4I Vec4_Add(Vec4I a, Vec4I b);

#endif