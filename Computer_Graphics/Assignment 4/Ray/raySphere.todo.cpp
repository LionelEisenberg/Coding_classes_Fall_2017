#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "rayScene.h"
#include "raySphere.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RaySphere::intersect(Ray3D ray,RayIntersectionInfo& iInfo,double mx){
	return -1;
}
BoundingBox3D RaySphere::setBoundingBox(void){
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////
int RaySphere::drawOpenGL(int materialIndex){
	if (materialIndex != material->index)
		material->drawOpenGL();
	glTranslatef(center[0], center[1], center[2]);
	GLUquadric* q = gluNewQuadric();
	gluSphere(q, radius, openGLComplexity, openGLComplexity);
	gluDeleteQuadric(q);
	return material->index;
}
