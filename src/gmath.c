#include "gmath.h"

// A float representation of PI.
static const float PI = 3.1415926535897932384626f;
// A float representation of PI/2
static const float PI_HALF = 1.570796326794896619231f;

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
 * Slightly modifid version of: 
 * 	https://stackoverflow.com/a/36387954
 */
float fast_acos(float x) {
	float sq = x*x;
	return PI_HALF + x * (0.9217841528914573f * sq - 0.939115566365855f) / 
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
 * radians rad => radians * 180° / PI
 */
float rad_to_deg(float radians) {
	return 180.0f * radians / PI;
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