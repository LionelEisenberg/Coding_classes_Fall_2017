#include <math.h>
/////////////////////
// Animation stuff //
/////////////////////

template <class Vector>
void ParameterSamples<Vector>::setCurrentValue(double t,int type){
	int floorSample = floor(t * count);
	int ceilSample = ceil(t * count);
	float s = 0.5;
	float t3 = pow(t, 3);
	float t2 = pow(t, 2);
	switch (type){
		case LINEAR:
			currentValue = (samples[ceilSample] - samples[floorSample])/(ceilSample - floorSample) * t + samples[floorSample];
			break;
		case CATMULL_ROM:
			float catMull[4];
			catMull[0] = -s*t3 + 2*s*t2 - s*t;
			catMull[1] = (2-s)*t3 + (s-3)*t2 + 1;
			catMull[2] = (s-2)*t3 + (3-2*s)*t2 + s*t;
			catMull[3] = s*t3 - s*t2;
			currentValue = samples[floorSample - 1] * catMull[0] + samples[floorSample] * catMull[1] + samples[ceilSample] * catMull[2] + samples[ceilSample + 1] * catMull[3];
			break;
		case UNIFORM_CUBIC_B_SPLINE:
			float cubicB[4];
			cubicB[0] = (1.0/3.0-s)*t3 + (-1.0/2.0+2.0*s)*t2 - s*t + 1.0/6.0;
			cubicB[1] = (1-s)*t3 + (s-3.0/2.0)*t2 + 2.0/3.0;
			cubicB[2] = (s-1)*t3 + (3.0/2.0-2.0*s)*t2 + s*t + 1.0/6.0;
			cubicB[3] = (-1.0/3.0+s) * t3 + (1.0/2.0 - s) * t2;
			currentValue = samples[floorSample - 1] * cubicB[0] + samples[floorSample] * cubicB[1] + samples[ceilSample] * cubicB[2] + samples[ceilSample + 1] * cubicB[3];
			break;
	};
}
