#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "rayGroup.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////

double RayGroup::intersect(Ray3D ray,RayIntersectionInfo& iInfo,double mx){
	//default return is no hit - so return -1
	double valToReturn = -1.0;
	Ray3D rayPrime = Ray3D(this->getInverseMatrix().multPosition(ray.position), (this->getInverseMatrix().multDirection(ray.direction)).unit());
	int totShapeHit = 0;
	//loop through shapes

	for(int i = 0; i < this->sNum; i++) {
		double dist1 = this->shapes[i]->bBox.intersect(rayPrime);
		if (dist1 >= 0 && (mx < 0 || dist1 < mx)) {
			this->hits[totShapeHit].shape = this->shapes[i];
			this->hits[totShapeHit].t = dist1;
			totShapeHit++;
		}
	}

	qsort(hits, totShapeHit, sizeof(RayShapeHit), RayShapeHit::Compare);

	for(int i = 0; i < totShapeHit; i++) {
		double dist = this->hits[i].shape->intersect(rayPrime, iInfo, mx);
		if (dist >= 0 && (mx < 0 || dist < mx)) {
			mx = dist;
			iInfo.iCoordinate = getMatrix().multPosition(iInfo.iCoordinate);
			iInfo.normal = getNormalMatrix().multDirection(iInfo.normal).unit();
			valToReturn = (iInfo.iCoordinate - ray.position).length();
		}
	}
	return valToReturn;
}

BoundingBox3D RayGroup::setBoundingBox(void){
	double xmax = -10000;
	double ymax = -10000;
	double zmax = -10000;
	double xmin = 10000;
	double ymin = 10000;
	double zmin = 10000;
	for (int i = 0; i < this->sNum; i++) {
		this->shapes[i]->setBoundingBox();
		BoundingBox3D b = this->shapes[i]->bBox;
		if (xmax < b.p[1].p[0]) {
			xmax = b.p[1].p[0];
		}
		if (ymax < b.p[1].p[1]) {
			ymax = b.p[1].p[1];
		}
		if (zmax < b.p[1].p[2]) {
			zmax = b.p[1].p[2];
		}
		if (xmin > b.p[0].p[0]) {
			xmin = b.p[0].p[0];
		}
		if (ymin > b.p[0].p[1]) {
			ymin = b.p[0].p[1];
		}
		if (zmin > b.p[0].p[2]) {
			zmin = b.p[0].p[2];
		}
	}
	this->bBox.p[0] = Point3D(xmin, ymin, zmin);
	this->bBox.p[1] = Point3D(xmax, ymax, zmax);
	return bBox;
}

int StaticRayGroup::set(void){
	this->inverseTransform = this->localTransform.invert();
	this->normalTransform = this->localTransform.transpose().invert();
	return 1;
}
//////////////////
// OpenGL stuff //
//////////////////

int RayGroup::drawOpenGL(int materialIndex){
	return -1;
}

/////////////////////
// Animation Stuff //
/////////////////////
Matrix4D ParametrizedEulerAnglesAndTranslation::getMatrix(void){
	return Matrix4D::IdentityMatrix();
}
Matrix4D ParametrizedClosestRotationAndTranslation::getMatrix(void){
	return Matrix4D::IdentityMatrix();
}
Matrix4D ParametrizedRotationLogarithmAndTranslation::getMatrix(void){
	return Matrix4D::IdentityMatrix();
}
Matrix4D ParametrizedQuaternionAndTranslation::getMatrix(void){
	return Matrix4D::IdentityMatrix();
}
