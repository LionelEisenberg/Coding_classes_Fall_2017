#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "rayScene.h"
#include "rayBox.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RayBox::intersect(Ray3D ray,RayIntersectionInfo& iInfo,double mx){
	return -1;
}
BoundingBox3D RayBox::setBoundingBox(void){
	bBox=BoundingBox3D(center-(length/2),center+(length/2));
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////

//https://www.opengl.org/archives/resources/code/samples/glut_examples/examples/cube.c
int RayBox::drawOpenGL(int materialIndex){
	glDisable(GL_CULL_FACE);
		GLfloat norms[6][3] = {
				{-1.0, 0.0, 0.0},
				{0.0, 1.0, 0.0},
				{1.0, 0.0, 0.0},
				{0.0, -1.0, 0.0},
				{0.0, 0.0, 1.0},
				{0.0, 0.0, -1.0}};
			GLint f[6][4] = {
				{0, 1, 2, 3},
				{3, 2, 6, 7},
				{7, 6, 5, 4},
				{4, 5, 1, 0},
				{5, 6, 2, 1},
				{7, 4, 0, 3} };
			GLfloat v[8][3];

			v[0][0] = v[1][0] = v[2][0] = v[3][0] =  - this->length[0] / 2 + this->center[0];
			v[0][1] = v[1][1] = v[4][1] = v[5][1] =  - this->length[1] / 2 + this->center[1];
			v[0][2] = v[3][2] = v[4][2] = v[7][2] =  - this->length[2] / 2 + this->center[2];
			v[4][0] = v[5][0] = v[6][0] = v[7][0] = this->length[0] / 2 + this->center[0];
			v[2][1] = v[3][1] = v[6][1] = v[7][1] = this->length[1] / 2 + this->center[1];
			v[1][2] = v[2][2] = v[5][2] = v[6][2] = this->length[2] / 2 + this->center[2];

			if (materialIndex != this->material->index) {
				this->material->drawOpenGL();
			}
		  for (int i = 0; i < 6; i++) {
		    glBegin(GL_QUADS);
		    glNormal3fv(&norms[i][0]);
				for (int j = 0; j < 4; j++) {
					glVertex3fv(&v[f[i][j]][0]);
				}
		    glEnd();
		  }

	return materialIndex;
}
