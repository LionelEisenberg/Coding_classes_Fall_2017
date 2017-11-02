#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "rayScene.h"
#include "raySphere.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RaySphere::intersect(Ray3D ray, RayIntersectionInfo& iInfo,double mx){
	Point3D pos = ray.position;
	Point3D dir = ray.direction.unit();
	double alpha, beta, gamma, discriminant;
	Point3D intersection;

	alpha = 1.0;
	beta = 2 * dir.dot((pos - this->center));
	gamma = pow((pos - this->center).length(), 2) - pow(this->radius, 2);
	discriminant = pow(beta, 2) - 4 * alpha * gamma;

	if (discriminant < 0) {
		return -1;
	} else if (discriminant == 0) {
		double t = (-beta + sqrt(discriminant)) / (2 * alpha);
		if (t < 0.0001) {
			return -1;
		}
		intersection = pos + dir * t;
		if (mx < 0 || (intersection - pos).length() < mx) {
			iInfo.iCoordinate = intersection;
			iInfo.material = this->material;
			iInfo.normal = (intersection - this->center).unit();
		}
		return (intersection - pos).length();
	} else {
		double t1 = (-beta + sqrt(discriminant)) / (2 * alpha);
		double t2 = (-beta - sqrt(discriminant)) / (2 * alpha);
		Point3D intersection1 = pos + dir * t1;
		Point3D intersection2 = pos + dir * t2;
		if ((intersection1 - pos).length() < (intersection2 - pos).length() && t1 > 0.0001) {
			intersection = intersection1;
			if (t1 < 0.0001) {
				return -1;
			}
		} else {
			intersection = intersection2;
			if (t2 < 0.0001) {
				return -1;
			}
		}
		if (mx < 0 || (intersection - pos).length() < mx) {
			iInfo.iCoordinate = intersection;
			iInfo.material = this->material;
			iInfo.normal = (intersection - this->center).unit();
		}
		return (intersection - pos).length();
	}
}

BoundingBox3D RaySphere::setBoundingBox(void){
 	double xmax = this->center.p[0] + radius;
	double ymax = this->center.p[1] + radius;
	double zmax = this->center.p[2] + radius;
	double xmin = this->center.p[0] - radius;
	double ymin = this->center.p[1] - radius;
	double zmin = this->center.p[2] - radius;
	this->bBox.p[0] = Point3D(xmin, ymin, zmin);
	this->bBox.p[1] = Point3D(xmax, ymax, zmax);
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////
int RaySphere::drawOpenGL(int materialIndex){
	return -1;
}
