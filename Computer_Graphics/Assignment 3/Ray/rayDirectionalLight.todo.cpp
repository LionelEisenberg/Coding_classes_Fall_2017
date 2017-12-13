#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "rayDirectionalLight.h"
#include "rayScene.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
Point3D RayDirectionalLight::getDiffuse(Point3D cameraPosition,RayIntersectionInfo& iInfo){
	return Point3D();
}
Point3D RayDirectionalLight::getSpecular(Point3D cameraPosition,RayIntersectionInfo& iInfo){
	return Point3D();
}
int RayDirectionalLight::isInShadow(RayIntersectionInfo& iInfo,RayShape* shape){
	return 0;
}
Point3D RayDirectionalLight::transparency(RayIntersectionInfo& iInfo,RayShape* shape,Point3D cLimit){
	return Point3D(1,1,1);
}

//////////////////
// OpenGL stuff //
//////////////////
void RayDirectionalLight::drawOpenGL(int index){
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0+index);
	GLfloat Color[] = {color[0], color[1], color[2], 0.0};
	GLfloat position[] = {-direction[0], -direction[1], -direction[2], 0.0};
	glLightfv(GL_LIGHT0+index, GL_POSITION, position);
	glLightfv(GL_LIGHT0+index, GL_DIFFUSE, Color);
	glLightfv(GL_LIGHT0+index, GL_SPECULAR, Color);
}
