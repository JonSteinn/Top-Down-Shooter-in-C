#include "gmath.h"


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

float fast_acos(float x) {
	return (-0.69813170079773212f * x * x - 0.87266462599716477f) * x + 1.5707963267948966f;
}


float rad_to_deg(float rad) {
	return 180.0f * rad / gPI;
}

int sign(float x) {
	return x < 0 ? -1 : x != 0;
}