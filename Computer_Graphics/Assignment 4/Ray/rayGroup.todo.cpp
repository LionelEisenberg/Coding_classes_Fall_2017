#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "rayGroup.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RayGroup::intersect(Ray3D ray,RayIntersectionInfo& iInfo,double mx){
	return -1;
}

BoundingBox3D RayGroup::setBoundingBox(void){
	return bBox;
}

int StaticRayGroup::set(void){
	return 1;
}
//////////////////
// OpenGL stuff //
//////////////////

int RayGroup::drawOpenGL(int materialIndex){
	glMatrixMode(GL_MODELVIEW);
	for (int x = 0; x < shapeNum(); x++) {
		glPushMatrix();
		Matrix4D M = getMatrix();
		GLfloat m[16] = {
			(GLfloat)M(0,0), (GLfloat)M(0,1), (GLfloat)M(0,2), (GLfloat)M(0,3),
			(GLfloat)M(1,0), (GLfloat)M(1,1), (GLfloat)M(1,2), (GLfloat)M(1,3),
			(GLfloat)M(2,0), (GLfloat)M(2,1), (GLfloat)M(2,2), (GLfloat)M(2,3),
			(GLfloat)M(3,0), (GLfloat)M(3,1), (GLfloat)M(3,2), (GLfloat)M(3,3)
		};
		glMultMatrixf(m);
		shapes[x]->drawOpenGL(materialIndex);
		glPopMatrix();
	}
	return materialIndex;
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
