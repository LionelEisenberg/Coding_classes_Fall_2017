#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "rayScene.h"
#include <algorithm>
#include "rayBox.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RayBox::intersect(Ray3D ray,RayIntersectionInfo& iInfo,double mx){
	Point3D* p;
	p[0] = this->center - length / 2;
	p[1] = this->center + length / 2;
	double tmin = -INFINITY, tmax = INFINITY;
	double tx1 = (p[0].p[0] - ray.position.p[0])/ray.direction.p[0];
	double tx2 = (p[1].p[0] - ray.position.p[0])/ray.direction.p[0];

	tmin = std::max(tmin, std::min(tx1, tx2));
	tmax = std::min(tmax, std::max(tx1, tx2));
	double ty1 = (p[0].p[1] - ray.position.p[1])/ray.direction.p[1];
	double ty2 = (p[1].p[1] - ray.position.p[1])/ray.direction.p[1];

	tmin = std::max(tmin, std::min(ty1, ty2));
	tmax = std::min(tmax, std::max(ty1, ty2));
	double tz1 = (p[0].p[2] - ray.position.p[2])/ray.direction.p[2];
	double tz2 = (p[1].p[2] - ray.position.p[2])/ray.direction.p[2];

	tmin = std::max(tmin, std::min(tz1, tz2));
	tmax = std::min(tmax, std::max(tz1, tz2));
	if (tmax < tmin) {
		return -1;
	}
	Point3D intersection = ray.position + ray.direction.unit() * tmin;
	double dist = (intersection - ray.position).length();
	if (mx < 0 || (dist < mx)) {
		iInfo.iCoordinate = intersection;
		iInfo.material = this->material;
		if (iInfo.iCoordinate.p[0] == p[0].p[0]) {
			Point3D pPrime = Point3D(center.p[0] - length.p[0], center[1], center[2]);
			iInfo.normal = (pPrime - center).unit();
		}
		if (iInfo.iCoordinate.p[0] == p[1].p[0]) {
			Point3D pPrime = Point3D(center[0] + length.p[0], center[1], center[2]);
			iInfo.normal = (pPrime - center).unit();
		}
		if (iInfo.iCoordinate.p[1] == p[0].p[1]) {
			Point3D pPrime = Point3D(center[0], center[1] - length.p[1], center[2]);
			iInfo.normal = (pPrime - center).unit();
		}
		if (iInfo.iCoordinate.p[1] == p[1].p[1]) {
			Point3D pPrime = Point3D(center[0], center[1] + length.p[1], center[2]);
			iInfo.normal = (pPrime - center).unit();
		}
		if (iInfo.iCoordinate.p[2] == p[0].p[2]) {
			Point3D pPrime = Point3D(center[0], center[1], center[2] - length.p[2]);
			iInfo.normal = (pPrime - center).unit();
		}
		if (iInfo.iCoordinate.p[2] == p[1].p[2]) {
			Point3D pPrime = Point3D(center[0], center[1], center[2] - length.p[2]);
			iInfo.normal = (pPrime - center).unit();
		}
		return (intersection - ray.position).length();
	} else {
		return -1;
	}
}

BoundingBox3D RayBox::setBoundingBox(void){
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////
int RayBox::drawOpenGL(int materialIndex){
	return -1;
}
