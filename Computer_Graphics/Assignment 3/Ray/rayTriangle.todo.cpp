#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rayTriangle.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
void RayTriangle::initialize(void){
}
double RayTriangle::intersect(Ray3D ray,RayIntersectionInfo& iInfo,double mx){
	return -1;
}
BoundingBox3D RayTriangle::setBoundingBox(void){
	Point3D pList[3];
	pList[0]=v[0]->position;
	pList[1]=v[1]->position;
	pList[2]=v[2]->position;
	bBox=BoundingBox3D(pList,3);
	for(int i=0;i<3;i++){
		bBox.p[0][i]-=RAYEPS;
		bBox.p[1][i]+=RAYEPS;
	}
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////
int RayTriangle::drawOpenGL(int materialIndex){
	if (material->tex) {
		glEnable(GL_TEXTURE_2D);
		material->tex->setUpOpenGL();
	}
	this->material->drawOpenGL();
	glBegin(GL_TRIANGLES);
	glNormal3f(plane.normal	.p[0],plane.normal.p[1], plane.normal.p[2]);
	glTexCoord2f(this->v[0]->texCoordinate[0], this->v[0]->texCoordinate[2]);
	glVertex3f(this->v[0]->position[0], this->v[0]->position[1], this->v[0]->position[2]);
	glTexCoord2f(this->v[1]->texCoordinate[0], this->v[1]->texCoordinate[2]);
	glVertex3f(this->v[1]->position[0], this->v[1]->position[1], this->v[1]->position[2]);
	glTexCoord2f(this->v[2]->texCoordinate[0], this->v[2]->texCoordinate[2]);
	glVertex3f(this->v[2]->position[0], this->v[2]->position[1], this->v[2]->position[2]);
	glEnd();

	return -1;
}
