#include <stdlib.h>
#include <math.h>
#include <SVD/SVDFit.h>
#include <SVD/MatrixMNTC.h>
#include <algorithm>
#include "geometry.h"


///////////////////////
// Ray-tracing stuff //
///////////////////////

double BoundingBox3D::intersect(const Ray3D& ray) const {
	double tmin = -INFINITY, tmax = INFINITY;
	double tx1 = (this->p[0].p[0] - ray.position.p[0])/ray.direction.p[0];
	double tx2 = (this->p[1].p[0] - ray.position.p[0])/ray.direction.p[0];

	tmin = std::max(tmin, std::min(tx1, tx2));
	tmax = std::min(tmax, std::max(tx1, tx2));
	double ty1 = (this->p[0].p[1] - ray.position.p[1])/ray.direction.p[1];
	double ty2 = (this->p[1].p[1] - ray.position.p[1])/ray.direction.p[1];

	tmin = std::max(tmin, std::min(ty1, ty2));
	tmax = std::min(tmax, std::max(ty1, ty2));
	double tz1 = (this->p[0].p[2] - ray.position.p[2])/ray.direction.p[2];
	double tz2 = (this->p[1].p[2] - ray.position.p[2])/ray.direction.p[2];

	tmin = std::max(tmin, std::min(tz1, tz2));
	tmax = std::min(tmax, std::max(tz1, tz2));
	if (tmin <= 0 && tmax >= 0) {
		return 0;
	}
 	if (tmax >= fmin(tmin, 0.0)) {
		return (ray.direction * tmin).length();
	} else {
		return -1;
	}
}

/////////////////////
// Animation stuff //
/////////////////////
Matrix3D::Matrix3D(const Point3D& e){
	(*this)=Matrix3D();
}

Matrix3D::Matrix3D(const Quaternion& q){
	(*this)=Matrix3D();
}
Matrix3D Matrix3D::closestRotation(void) const {
	return (*this);
}
/* While these Exp and Log implementations are the direct implementations of the Taylor series, the Log
 * function tends to run into convergence issues so we use the other ones:*/
Matrix3D Matrix3D::Exp(const Matrix3D& m,int iter){
	return m;
}
