#ifndef VECTOR2_H
#define VECTOR2_H

typedef struct vector2{
    int x;
    int y;
} Vector2;

typedef struct vector4{
    int l;
    int r;
    int t;
    int b;
} Vector4;

Vector2 Vec2_Add(Vector2 a, Vector2 b);
Vector4 Vec4_Add(Vector4 a, Vector4 b);

#endif