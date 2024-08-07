#ifndef VEC2I_H
#define VEC2I_H

// Return the lesser of two inputs
#define min(a,b) (a < b ? a : b)

// Return the greater of two inputs
#define max(a,b) (a > b ? a : b)

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

#define VEC2_ZERO ((Vec2I){0,0})
#define VEC4_ZERO ((Vec2I){0,0,0,0})

#endif