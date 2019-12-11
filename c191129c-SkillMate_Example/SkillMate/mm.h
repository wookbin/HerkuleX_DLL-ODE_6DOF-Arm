#pragma once

#include <math.h>

#ifndef M_PI
#define M_PI		3.14159265358979323846	// pi 
#endif

#define _RAD2DEG	(180./M_PI)
#define _DEG2RAD	(M_PI/180.)

inline double DeltaRad (double ang1, double ang2)
{
	double da = ang1 - ang2;
	if (-M_PI < da && da < M_PI) return da;
	else {
		da = fmod(da, 2.0*M_PI);
		if (M_PI <= da) return da - 2*M_PI;
		else if (da <= -M_PI) return da + 2*M_PI;
		else return da;
	}
	return da;
}
