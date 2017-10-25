#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rayTriangle.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
void RayTriangle::initialize(void){
	Plane3D* plane = new Plane3D(this->v[0]->position, this->v[1]->position,this->v[2]->position);
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
	return bBox;
}



//////////////////
// OpenGL stuff //
//////////////////
int RayTriangle::drawOpenGL(int materialIndex){
	return -1;
}
