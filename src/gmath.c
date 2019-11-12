#include "gmath.h"


// A float representation of PI.
static const float PI = 3.1415926535897932384626f;
// A float representation of PI/2
static const float HALF_PI = 1.570796326794896619231f;
// A float representation of 2*PI
static const float TWO_PI = 6.283185f;
// A float representation of 1/(2*Pi)
static const float TWO_PI_INVERSE = 0.1591549f;
// A float representation of 3*PI/2
static const float THREE_HALF_PI = 4.7123889f;
// A float representation of 180 / PI
static const float DEGREES_IN_ONE_RADIAN = 57.29577951308232f;

/**
 * Function:
 * 	__cos_helper
 *
 * Purpose:
 * 	Estimate cos with partial series expansion.
 *
 * Parameters:
 * 	- x:
 * 		Angle in radians
 *
 * Returns:
 * 	Series expansion of cos(x).
 */
static float __cos_helper(float x);


/**
 * John Carmack's infamous inverse sqrt from Quake3 Arena.
 * (Not really his but he famously utilised it)
 *
 * Preprocessor directive added to supress warnings the
 * original code produces, otherwise directly taken from
 * Quake3's source.
 */
float carmack_inverse_sqrt(float x)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = x * 0.5F;
	y  = x;
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wstrict-aliasing"
	i  = * (( long * ) &y);                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
	y  = * ( float * ) &i;
	#pragma GCC diagnostic pop
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
	//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}

/**
 * Borrowed from:
 * 	https://github.com/divideconcept/FastTrigo/blob/master/fasttrigo.cpp
 */
float fast_cos(float x) {

    x = x - floorf(x * TWO_PI_INVERSE) * TWO_PI;
    x = x > 0.0f ? x : -x;

    if(x < HALF_PI) return __cos_helper(x);
    if(x < PI) return -__cos_helper(PI - x);
    if(x < THREE_HALF_PI) return -__cos_helper(x - PI);
    return __cos_helper(TWO_PI - x);
}

/**
 * Uses fast_cos to compute by shifting angle.
 */
float fast_sin(float x) {
	return fast_cos(HALF_PI - x);
}

/**
 * Slightly modifid version of:
 * 	https://stackoverflow.com/a/36387954
 */
float fast_acos(float x) {
	float sq = x*x;
	return HALF_PI + x * (0.9217841528914573f * sq - 0.939115566365855f) /
		(1 + sq * (0.295624144969963174f * sq - 1.2845906244690837f));
}

/**
 * A circle is 360 degress or 2*PI radians, thus
 *
 * 2*PI rad = 360°
 * =>
 * PI rad = 180°
 * =>
 * 1 rad = 180° / PI
 * =>
 * radians rad = radians * 180° / PI
 */
float rad_to_deg(float radians) {
	return DEGREES_IN_ONE_RADIAN * radians;
}

/**
 * if (x < 0) {
 *     return -1;
 * } else {
 * 	   return x != 0 // 1 if x > 0, 0 otherwise
 * }
 */
int sign(float x) {
	return x < 0 ? -1 : x != 0;
}

/**
 * Length without the sqrt.
 */
float length_squared(Vector2d* vec) {
    return vec->x * vec->x + vec->y * vec->y;
}

/**
 * Borrowed from:
 * 	https://github.com/divideconcept/FastTrigo/blob/master/fasttrigo.cpp
 */
static float __cos_helper(float x) {
    float x2 = x*x;
    return 0.99940307f + x2*(0.49558072f + 0.03679168f * x2);
}