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
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////
int RayCylinder::drawOpenGL(int materialIndex){
	if(materialIndex != material->index)
		material->drawOpenGL();

	GLUquadric* q = gluNewQuadric();
	gluQuadricTexture(q, true);

	glTranslatef(center.p[0], center.p[1] - height/2, center.p[2]);
	glRotatef(90, -1, 0, 0);
	gluCylinder(q, radius, radius, height, openGLComplexity, openGLComplexity);

	glPushMatrix();
	glTranslatef(0, 0, height);
	gluDisk(q, 0, radius, openGLComplexity, openGLComplexity);
	glPopMatrix();

	glRotatef(180, 1, 0, 0); // Normals pointing out
	gluDisk(q, 0, radius, openGLComplexity, openGLComplexity);

	return material->index;
}

