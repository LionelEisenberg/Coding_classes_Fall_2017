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
	Point3D e = this->value->eulerAngles;
	Point3D translate = this->value->translate;
	Matrix3D r = Matrix3D(e);
	Matrix4D trans = Matrix4D();
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			trans(i,j) = r(i,j);
		}
	}
	trans(3,3)=1;
	trans(0,3)=trans(1,3)=trans(2,3)=0;
	for (int i = 0; i < 3; i++) {
		trans(3,i) = translate.p[i];
	}
	// printf("%f %f %f\n", e[0],e[1],e[2]);
	// for (int i = 0; i < 4; i++) {
	// 	for (int j = 0; j < 4; j++) {
	// 		printf("%f	", trans(i,j));
	// 	}
	// 	printf("\n");
	// }
	return trans;
}
Matrix4D ParametrizedClosestRotationAndTranslation::getMatrix(void){
		Point3D translate = this->value->translate;
		Matrix3D r = this->value->rotation;
		Matrix4D trans = Matrix4D();
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				trans(i,j) = r(i,j);
			}
		}
		trans(3,3)=1;
		trans(0,3)=trans(1,3)=trans(2,3)=0;
		for (int i = 0; i < 3; i++) {
			trans(3,i) = translate.p[i];
		}
		// for (int i = 0; i < 4; i++) {
		// 	for (int j = 0; j < 4; j++) {
		// 		printf("%f	", trans(i,j));
		// 	}
		// 	printf("\n");
		// }
		return trans;
}

Matrix4D ParametrizedRotationLogarithmAndTranslation::getMatrix(void){
	return Matrix4D(Matrix3D::Exp(this->value->skewSymmetric, 100).closestRotation(), this->value->translate);
}

Matrix4D ParametrizedQuaternionAndTranslation::getMatrix(void){
	Point3D translate = this->value->translate;
	Quaternion q = this->value->quaternion;
	Matrix3D r = Matrix3D(q);
	Matrix4D trans = Matrix4D();
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			trans(i,j) = r(i,j);
		}
	}
	trans(3,3)=1;
	trans(0,3)=trans(1,3)=trans(2,3)=0;
	for (int i = 0; i < 3; i++) {
		trans(3,i) = translate.p[i];
	}
	// for (int i = 0; i < 4; i++) {
	// 	for (int j = 0; j < 4; j++) {
	// 		printf("%f	", trans(i,j));
	// 	}
	// 	printf("\n");
	// }
	return trans;
}
