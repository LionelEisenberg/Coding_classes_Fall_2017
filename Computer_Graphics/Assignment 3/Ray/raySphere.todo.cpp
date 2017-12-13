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
	Point3D p=Point3D(radius,radius,radius);
	bBox=BoundingBox3D(center+p,center-p);
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////
int RaySphere::drawOpenGL(int materialIndex){
	glDisable(GL_CULL_FACE);
	if (materialIndex != this->material->index) {
		this->material->drawOpenGL();
	}
	GLUquadric *quad;
	quad = gluNewQuadric();
	glTranslatef(this->center[0], this->center[1], this->center[2]);
	gluSphere(quad,this->radius,this->openGLComplexity,this->openGLComplexity);
	return this->material->index;
}
