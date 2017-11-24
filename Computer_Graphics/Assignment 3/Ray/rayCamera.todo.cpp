#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include "rayCamera.h"



//////////////////
// OpenGL stuff //
//////////////////

float toDegree(float angle) {
  return angle * 180.0 / PI;
}

void RayCamera::drawOpenGL(void){
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
	Point3D dir = position + direction;
  gluLookAt(this->position.p[0], this->position.p[1], this->position.p[2], dir[0], dir[1],dir[2], this->up[0], this->up[1], this->up[2]);
}

void RayCamera::rotateUp(Point3D center,float angle){
}
void RayCamera::rotateRight(Point3D center,float angle){
}
void RayCamera::moveForward(float dist){
}
void RayCamera::moveRight(float dist){
}
void RayCamera::moveUp(float dist){
}
