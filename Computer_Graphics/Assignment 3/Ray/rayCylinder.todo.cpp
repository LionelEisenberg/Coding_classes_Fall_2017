#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "rayScene.h"
#include "rayCylinder.h"


////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RayCylinder::intersect(Ray3D ray,RayIntersectionInfo& iInfo,double mx){
	return -1;
}

BoundingBox3D RayCylinder::setBoundingBox(void){
	Point3D p;
	p=Point3D(radius,height/2,radius);
	bBox=BoundingBox3D(center+p,center-p);
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////
int RayCylinder::drawOpenGL(int materialIndex){
	if (materialIndex != this->material->index) {
		this->material->drawOpenGL();
	}
	GLUquadric *quad, *quad1, *quad2;
	quad = gluNewQuadric();
	glTranslatef(this->center[0], this->center[1], this->center[2]);
	gluCylinder(quad, this->radius, this->radius, this->height , openGLComplexity, openGLComplexity);

	quad1 = gluNewQuadric();
	glTranslatef(0, 0, height);
	gluDisk(quad1,0,  this->radius, openGLComplexity, openGLComplexity);

	quad2 = gluNewQuadric();
	gluDisk(quad2,0,  this->radius, openGLComplexity, openGLComplexity);

	return this->material->index;
}
