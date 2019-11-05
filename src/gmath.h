#ifndef xJdkK3dms1_GMATH_H
#define xJdkK3dms1_GMATH_H

typedef struct {
    float x;
    float y;
} Vector2d;

typedef Vector2d Point2d;

const float gPI = 3.14159265358979f;


float carmack_inverse_sqrt(float x);
float fast_acos(float x);
float rad_to_deg(float rad);
int sign(float x);

#endif
