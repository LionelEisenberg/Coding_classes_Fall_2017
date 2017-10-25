#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "rayGroup.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////


double RayGroup::intersect(Ray3D ray,RayIntersectionInfo& iInfo,double mx){
	double valToReturn = -1.0;
	Ray3D rayPrime = Ray3D(this->getInverseMatrix().multPosition(ray.position), (this->getInverseMatrix().multDirection(ray.direction)).unit());
	for(int i = 0; i < this->sNum; i++) {
		double dist = this->shapes[i]->intersect(rayPrime, iInfo, mx);
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
