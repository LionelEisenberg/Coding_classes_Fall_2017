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

//https://gamedev.stackexchange.com/questions/149525/how-do-we-rotate-the-camera-around-x-axis-360-degrees
//https://www.youtube.com/watch?v=7oNLw9Bct1k
void RayCamera::rotateUp(Point3D center, float angle){
    this->position = this->position * cos(angle) + this->up.crossProduct(this->position) * sin(angle) + this->up * this->position.dot(this->up) * (1 - cos(angle));
    this->up = this->up * cos(angle) + this->up.crossProduct(this->up) * sin(angle) + this->up * this->up.dot(this->up) * (1 - cos(angle));
    this->direction = this->direction * cos(angle) + this->up.crossProduct(this->direction) * sin(angle) + this->up * this->direction.dot(this->up) * (1 - cos(angle));
}

void RayCamera::rotateRight(Point3D center, float angle){
  this->position = this->position * cos(angle) + this->right.crossProduct(this->position) * sin(angle) + this->right * this->position.dot(this->right) * (1 - cos(angle));
  this->up = this->up * cos(angle) + this->right.crossProduct(this->up) * sin(angle) + this->right * this->up.dot(this->right) * (1 - cos(angle));
  this->direction = this->direction * cos(angle) + this->right.crossProduct(this->direction) * sin(angle) + this->right * this->direction.dot(this->right) * (1 - cos(angle));
}

void RayCamera::moveForward(float dist){
  this->position = this->position + this->direction * dist;
}
void RayCamera::moveRight(float dist){
  this->position = this->position + this->right * dist;
}
void RayCamera::moveUp(float dist){
  this->position = this->position + this->up * dist;
}
