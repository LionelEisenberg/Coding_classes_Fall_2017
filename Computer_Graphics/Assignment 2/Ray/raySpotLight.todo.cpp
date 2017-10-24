#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "rayScene.h"
#include "raySpotLight.h"


////////////////////////
//  Ray-tracing stuff //
////////////////////////
Point3D RaySpotLight::getDiffuse(Point3D cameraPosition,RayIntersectionInfo& iInfo){
	Point3D AttenColor;
	Point3D position = this->location;
	Point3D v = (iInfo.iCoordinate - v).unit();
	Point3D d = this->direction;
	double dist = (iInfo.iCoordinate - position).length();
	double angle = pow(d.dot(v), this->dropOffRate);
	if (angle > cos(this->cutOffAngle)) {
		AttenColor = this->color * angle / (this->constAtten + this->linearAtten * dist + this->quadAtten * pow(dist, 2));
	} else {
		AttenColor = Point3D(0,0,0);
	}
	Point3D point = AttenColor * iInfo.normal.dot((position - iInfo.iCoordinate).unit()) * iInfo.material->diffuse;
	return point;
}
Point3D RaySpotLight::getSpecular(Point3D cameraPosition,RayIntersectionInfo& iInfo){
	return Point3D();
}
int RaySpotLight::isInShadow(RayIntersectionInfo& iInfo,RayShape* shape){
	return 0;
}
Point3D RaySpotLight::transparency(RayIntersectionInfo& iInfo,RayShape* shape,Point3D cLimit){
	return Point3D(1,1,1);
}

//////////////////
// OpenGL stuff //
//////////////////
void RaySpotLight::drawOpenGL(int index){
}