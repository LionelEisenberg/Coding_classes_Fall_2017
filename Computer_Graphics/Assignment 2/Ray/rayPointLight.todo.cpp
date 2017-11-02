#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "rayPointLight.h"
#include "rayScene.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
Point3D RayPointLight::getDiffuse(Point3D cameraPosition,RayIntersectionInfo& iInfo){
	Point3D position = this->location;
	double dist = (position - iInfo.iCoordinate).length();
	Point3D AttenColor = this->color / (this->constAtten + this->linearAtten * dist + this->quadAtten * pow(dist, 2));
	Point3D point = AttenColor * iInfo.normal.dot((position - iInfo.iCoordinate).unit()) * iInfo.material->diffuse;
	return point;
}

Point3D RayPointLight::getSpecular(Point3D cameraPosition,RayIntersectionInfo& iInfo){
	Point3D position = this->location;
	double dist = (position - iInfo.iCoordinate).length();
	Point3D AttenColor = this->color / (this->constAtten + this->linearAtten * dist + this->quadAtten * pow(dist, 2));
	Point3D viewerVector = (cameraPosition - iInfo.iCoordinate).unit();
	double alpha = iInfo.normal.unit().dot(((position - iInfo.iCoordinate).unit() + viewerVector).unit());
	Point3D point = iInfo.material->specular * pow(alpha, iInfo.material->specularFallOff) * AttenColor;
	return Point3D();
}
int RayPointLight::isInShadow(RayIntersectionInfo& iInfo,RayShape* shape){
	Point3D position = this->location;
	Ray3D ray = Ray3D(iInfo.iCoordinate, (position - iInfo.iCoordinate).unit());
	if (shape->intersect(ray, iInfo, -1.0) != -1 && shape->intersect(ray, iInfo, -1.0) < (position - iInfo.iCoordinate).length()) {
		return 1;
	}
	return 0;
}

Point3D RayPointLight::transparency(RayIntersectionInfo& iInfo,RayShape* shape,Point3D cLimit){
	Point3D position = this->location;
	Point3D transparency;
	Point3D contribution = Point3D(1,1,1);
	double dist;
	int count = 0;
	while (contribution[0] > cLimit[0]) {
		Ray3D ray = Ray3D(iInfo.iCoordinate, (position - iInfo.iCoordinate).unit());
		dist = shape->intersect(ray, iInfo, -1.0);
		if ((dist == -1  || dist > (position - iInfo.iCoordinate).length()) && count == 0) {
			return Point3D(1,1,1);
		}
		if (dist != -1 && dist < (position - iInfo.iCoordinate).length()) {
			Point3D kT = iInfo.material->transparent;
			contribution = kT * 1;
			transparency += truncate(contribution);
			count++;
		} else {
			break;
		}
	}
	return truncate(transparency);
}


//////////////////
// OpenGL stuff //
//////////////////
void RayPointLight::drawOpenGL(int index){
}