#include <math.h>
/////////////////////
// Animation stuff //
/////////////////////

template <class Vector>
void ParameterSamples<Vector>::setCurrentValue(double t,int type){
	int floorSample = (int) floor(t * count) % count;
	int ceilSample = (floorSample + 1) % count;
	float s = 0.5;
	float a = count * t - floorSample;
	float b = 1 - a;
	float t3 = pow(a, 3);
	float t2 = pow(a, 2);
	type = UNIFORM_CUBIC_B_SPLINE;
	switch (type){
		case LINEAR: {
			currentValue = samples[floorSample] * b + samples[(ceilSample) % count] * a;
			break;
		}
		case CATMULL_ROM: {
			float catMull[4];
		  catMull[0] = -s * t3 + 2 * s * t2 - s * a;
		  catMull[1] = (2 - s) * t3 + (s - 3) * t2 + 1;
		  catMull[2] = (s - 2) * t3 + (3 - 2 * s) * t2 + s * a;
		  catMull[3] = s * t3 - s * t2;
			currentValue = samples[(floorSample - 1) % count] * catMull[0] +  samples[floorSample] * catMull[1] + samples[(ceilSample) % count] * catMull[2] + samples[(ceilSample) % count] * catMull[3];
			break;
		}
		case UNIFORM_CUBIC_B_SPLINE:
			float cubicB[4];
			cubicB[0] = (1.0/3.0-s)*t3 + (-1.0/2.0+2.0*s)*t2 - s*a + 1.0/6.0;
			cubicB[1] = (1-s)*t3 + (s-3.0/2.0)*t2 + 2.0/3.0;
			cubicB[2] = (s-1)*t3 + (3.0/2.0-2.0*s)*t2 + s*a + 1.0/6.0;
			cubicB[3] = (-1.0/3.0+s) * t3 + (1.0/2.0 - s) * t2;
			currentValue = samples[(floorSample - 1) % count] * cubicB[0] + samples[floorSample] * cubicB[1] + samples[ceilSample] * cubicB[2] + samples[(ceilSample + 1) % count] * cubicB[3];
			break;
	};
}
