//day 2 : adimath.h
#ifndef ADIMATH_H
#define ADIMATH_H
// user defined types and functions declarations

typedef struct vector
{
    float x;
    float y;
}vector_h;

void Add(vector_h* out, vector_h* in);
#endif 