#include <stdlib.h>
#include <math.h>

#include <SVD/SVDFit.h>
#include <SVD/MatrixMNTC.h>

#include "geometry.h"


///////////////////////
// Ray-tracing stuff //
///////////////////////
double BoundingBox3D::intersect(const Ray3D& ray) const {
	return -1;
}

/////////////////////
// Animation stuff //
/////////////////////
void printMatrix(Matrix3D m) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			printf("%f	", m(i,j));
		}
		printf("\n");
	}
}

float sign(float l) {
	if (l >= 0) {
		return 1.0;
	} else if (l < 0) {
		return -1.0;
	}
}

Matrix3D::Matrix3D(const Point3D& e){
	Matrix3D r = Matrix3D();
	Matrix3D x = Matrix3D();
	Matrix3D y = Matrix3D();
	Matrix3D z = Matrix3D();

	x(0,0)= 1;
	x(0,1)=x(0,2)=x(1,0)=x(2,0)=0;
	x(1,1)=x(2,2)=cos(e.p[0]);
	x(2,1)=-sin(e.p[0]);
	x(1,2)=sin(e.p[0]);

	y(1,1)=1;
	y(0,1)=y(1,0)=y(2,1)=y(1,2)=0;
	y(0,0)=y(2,2)=cos(e.p[1]);
	y(2,0)=sin(e.p[1]);
	y(0,2)=-sin(e.p[1]);

	z(2,2)=1;
	z(2,1)=z(2,0)=z(0,2)=z(1,2)=0;
	z(0,0)=z(1,1)=cos(e.p[2]);
	z(0,1)=sin(e.p[2]);
	z(1,0)=-sin(e.p[2]);

	r = z * y * x;
	(*this)=r;
}

Matrix3D::Matrix3D(const Quaternion& q){
	q.unit();
	double a = q.real;
	double b = q.imag.p[0];
	double c = q.imag.p[1];
	double d = q.imag.p[2];
	Matrix3D r = Matrix3D();
	r(0,0) = 1 - 2*pow(c,2) - 2*pow(d,2);
	r(0,1) = 2*b*c + 2*a*d;
	r(0,2) = 2*b*d-2*a*c;
	r(1,0) = 2*b*c-2*a*d;
	r(1,1) = 1 - 2*pow(b,2) - 2*pow(d,2);
	r(1,2) = 2*c*d+2*a*b;
	r(2,0) = 2*b*d + 2*a*c;
	r(2,1) = 2*c*d - 2*a*b;
	r(2,2) = 1 - 2*pow(b,2) - 2*pow(c,2);
	(*this)=r;
}

Matrix3D Matrix3D::closestRotation(void) const {
	Matrix3D r1 = Matrix3D();
	Matrix3D r2 = Matrix3D();
	Matrix3D d = Matrix3D();
	Matrix3D dSign = Matrix3D();
	Matrix3D r = Matrix3D();

	this->SVD(r1, d, r2);
	for (int i = 0; i < 3; i++) {
		dSign(i,i) = sign(d(i,i));
	}
	r = r1 * dSign * r2;
	return r;
}
/* While these Exp and Log implementations are the direct implementations of the Taylor series, the Log
 * function tends to run into convergence issues so we use the other ones:*/
Matrix3D Matrix3D::Exp(const Matrix3D& m,int iter){
	Matrix3D r = Matrix3D::IdentityMatrix();
	Matrix3D mP = Matrix3D::IdentityMatrix();
	float factorial = 1;
	for (int k = 1; k < iter; k++) {
		mP *= m;
		factorial *= k;
		r += (mP / factorial);
	}
	return r;
}
