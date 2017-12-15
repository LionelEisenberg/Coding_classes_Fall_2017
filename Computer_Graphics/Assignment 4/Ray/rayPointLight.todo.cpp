#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "rayPointLight.h"
#include "rayScene.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
Point3D RayPointLight::getDiffuse(Point3D cameraPosition,RayIntersectionInfo& iInfo){
	return Point3D();
}
Point3D RayPointLight::getSpecular(Point3D cameraPosition,RayIntersectionInfo& iInfo){
	return Point3D();
}
int RayPointLight::isInShadow(RayIntersectionInfo& iInfo,RayShape* shape){
	return 0;
}
Point3D RayPointLight::transparency(RayIntersectionInfo& iInfo,RayShape* shape,Point3D cLimit){
	return Point3D(1,1,1);
}


//////////////////
// OpenGL stuff //
//////////////////
void RayPointLight::drawOpenGL(int index){
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0 + index);
	GLfloat Color[] = {color[0], color[1], color[2], 0.0};
	GLfloat position[] = {location[0], location[1], location[2], 1.0};
	GLfloat direction[] = {direction[0], direction[1], direction[2]};
	GLfloat constAttenuation[] = {constAtten};
	GLfloat linearAttenuation[] = {linearAtten};
	GLfloat quadAttenuation[] = {quadAtten};
	glLightfv(GL_LIGHT0+index, GL_DIFFUSE, Color);
	glLightfv(GL_LIGHT0+index, GL_SPECULAR, Color);
	glLightfv(GL_LIGHT0+index, GL_POSITION, position);
	glLightfv(GL_LIGHT0+index, GL_SPOT_DIRECTION, direction);
	glLightfv(GL_LIGHT0+index, GL_CONSTANT_ATTENUATION, constAttenuation);
	glLightfv(GL_LIGHT0+index, GL_QUADRATIC_ATTENUATION, quadAttenuation);
	glLightfv(GL_LIGHT0+index, GL_LINEAR_ATTENUATION, linearAttenuation);
}
