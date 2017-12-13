#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "rayScene.h"
#include "rayCone.h"


////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RayCone::intersect(Ray3D ray,RayIntersectionInfo& iInfo,double mx){
	return -1;
}

BoundingBox3D RayCone::setBoundingBox(void){
	Point3D p;
	p=Point3D(radius,height/2,radius);
	bBox=BoundingBox3D(center+p,center-p);
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////
int RayCone::drawOpenGL(int materialIndex){
	glDisable(GL_CULL_FACE);
	if (materialIndex != this->material->index) {
		this->material->drawOpenGL();
	}
	GLUquadric *quad, *quad1;
	quad = gluNewQuadric();
	glTranslatef(this->center[0], this->center[1], this->center[2]);
	gluCylinder(quad, this->radius, 0, this->height , openGLComplexity, openGLComplexity);

	quad1 = gluNewQuadric();
	gluDisk(quad1,0,  this->radius, openGLComplexity, openGLComplexity);
	return -1;
}
