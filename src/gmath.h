#ifndef xJdkK3dms1_GMATH_H
#define xJdkK3dms1_GMATH_H

/**
 * Struct:
 *  Coordinates2d
 * 
 * Purpose:
 *  A two element geometric tuple.
 * 
 * Fields:
 *  - x:
 *      The horizontal part of the coordinate.
 *  - y:
 *      The vertical part of the coordinate.
 */
typedef struct {
    float x;
    float y;
} Coordinates2d;

// A 2d movement.
typedef Coordinates2d Vector2d;
// A 2d position.
typedef Coordinates2d Point2d;

/**
 * Function:
 *  carmack_inverse_sqrt
 * 
 * Purpose:
 *  Approximate a number to the power of -1/2 quickly.
 * 
 * Parameters:
 *  - x:
 *      A positive number.
 * 
 * Returns:
 *  1/sqrt(x)
 */
float carmack_inverse_sqrt(float x);

/**
 * Function:
 *  fast_acos
 * 
 * Purpose:
 *  Approximate the arccos of a number quickly.
 * 
 * Parameters:
 *  - x:
 *      A number between -1 and 1.
 * 
 * Returns:
 *  arccos(x)
 */
float fast_acos(float radians);

/**
 * Function:
 *  rad_to_deg
 * 
 * Purpose:
 *  Convert radians to degrees.
 * 
 * Parameters:
 *  - radians:
 *      An angle in radians.
 * 
 * Returns:
 *  The angle as degres.
 */
float rad_to_deg(float rad);

/**
 * Function:
 *  sign
 * 
 * Purpose:
 *  Determine if a number is positive or negative (or neither).
 * 
 * Parameters:
 *  - x:
 *      A number.
 * 
 * Returns:
 *  1 if positive, -1 if negative, 0 otherwise.
 */
int sign(float x);

/**
 * Function:
 *  length_squared
 * 
 * Purpose:
 *  Compute the length of a vector to the second power.
 * 
 * Parameters:
 *  - vec:
 *      A 2d vector.
 * 
 * Returns:
 *  L*L given that the length of vec is L.
 */
float length_squared(Vector2d* vec);

#endif