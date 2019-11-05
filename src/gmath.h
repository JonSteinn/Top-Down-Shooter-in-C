#ifndef xJdkK3dms1_GMATH_H
#define xJdkK3dms1_GMATH_H

/**
 * A two value tuple.
 * 
 *  x
 *      The horizontal coordinate
 *  y
 *      The vertical coordinate
 */
typedef struct {
    float x;
    float y;
} Vector2d;

typedef Vector2d Point2d;

static const float gPI = 3.14159265358979f;

/**
 * John Carmack's infamous inverse sqrt from Quake3 Arena.
 */
float carmack_inverse_sqrt(float x);

/**
 * A fast approximation of arccos.
 */
float fast_acos(float x);

/**
 * Converts radians to degrees.
 */
float rad_to_deg(float rad);

/**
 * Returns 1 if x is positive, -1 if x is negative and 0 otherwise.
 */
int sign(float x);

#endif
