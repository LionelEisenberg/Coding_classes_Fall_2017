#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rayTriangle.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
void RayTriangle::initialize(void){
	this->v1 = this->v[0]->position;
	this->v2 = this->v[1]->position;
	Point3D v1 = (this->v[1]->position - this->v[0]->position);
	Point3D v2 = (this->v[2]->position - this->v[0]->position);
	this->plane.normal = v1.crossProduct(v2).unit();
	this->plane.distance = this->plane.normal.dot(this->v[0]->position);
}

double RayTriangle::intersect(Ray3D ray,RayIntersectionInfo& iInfo,double mx){
	Point3D edge1, edge2, edge3, p1, p2, p3;
	Point3D v0 = this->v[0]->position; Point3D v1 = this->v[1]->position; Point3D v2 = this->v[2]->position;
	double dotPosNorm, dotDirNorm;
	Point3D pos = ray.position; Point3D dir = ray.direction; Point3D norm = this->plane.normal;
	dotPosNorm = pos.dot(norm);
	dotDirNorm = dir.dot(norm);
	double t = - (dotPosNorm + this->plane.distance) / dotDirNorm;
	if (t < 0.0001) {
		return -1;
	}
	Point3D intersection = pos + dir * t;

	edge1 = v1 - v0;
	edge2 = v2 - v1;
 	edge3 = v0 - v2;
	p1 = intersection - v0;
	p2 = intersection - v1;
	p3 = intersection - v2;

	if(norm.dot(edge1.crossProduct(p1)) >= 0 && norm.dot(edge2.crossProduct(p2)) >= 0 && norm.dot(edge3.crossProduct(p3)) >= 0) {
		if(mx < 0 || (intersection - pos).length() < mx) {
			iInfo.iCoordinate = intersection;
			iInfo.material = this->material;
			iInfo.normal = norm.unit();
		}
		return (intersection - pos).length();
	}
	return -1;
}


BoundingBox3D RayTriangle::setBoundingBox(void){
	double xmax = -10000;
	double ymax = -10000;
	double zmax = -10000;
	double xmin = 10000;
	double ymin = 10000;
	double zmin = 10000;
	for (int i = 0; i < 3; i++) {
		if (xmax < this->v[i]->position[0]) {
			xmax = this->v[i]->position[0];
		}
		if (ymax < this->v[i]->position[1]) {
			ymax = this->v[i]->position[1];
		}
		if (zmax < this->v[i]->position[2]) {
			zmax = this->v[i]->position[2];
		}
		if (xmin > this->v[i]->position[0]) {
			xmin = this->v[i]->position[0];
		}
		if (ymin > this->v[i]->position[1]) {
			ymin = this->v[i]->position[1];
		}
		if (zmin > this->v[i]->position[2]) {
			zmin = this->v[i]->position[2];
		}
	}
	this->bBox.p[0] = Point3D(xmin, ymin, zmin);
	this->bBox.p[1] = Point3D(xmax, ymax, zmax);
	return bBox;
}



//////////////////
// OpenGL stuff //
//////////////////
int RayTriangle::drawOpenGL(int materialIndex){
	return -1;
}
