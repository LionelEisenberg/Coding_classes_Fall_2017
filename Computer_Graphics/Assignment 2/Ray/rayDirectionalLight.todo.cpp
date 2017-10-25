#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "rayDirectionalLight.h"
#include "rayScene.h"

// double abs(double d) {
// 	if (d < 0) {
// 		return -d;
// 	}
// 	return d;
// }

////////////////////////
//  Ray-tracing stuff //
////////////////////////
Point3D RayDirectionalLight::getDiffuse(Point3D cameraPosition,RayIntersectionInfo& iInfo){
	Point3D point = this->color * iInfo.normal.unit().dot(this->direction.negate()) * iInfo.material->diffuse;
	return point;
}
Point3D RayDirectionalLight::getSpecular(Point3D cameraPosition,RayIntersectionInfo& iInfo){
	Point3D viewerVector = (cameraPosition - iInfo.iCoordinate).unit();
	double alpha = iInfo.normal.unit().dot((direction.negate().unit() + viewerVector).unit());
	Point3D point = iInfo.material->specular * pow(alpha, iInfo.material->specularFallOff) * this->color;
	return point;
}

int RayDirectionalLight::isInShadow(RayIntersectionInfo& iInfo,RayShape* shape){
	// Point3D p = this->direction.unit().negate() * 100;
	// Ray3D ray = Ray3D(p, (iInfo.iCoordinate - p));
	Ray3D ray = Ray3D(iInfo.iCoordinate, this->direction.unit().negate());
	if (shape->intersect(ray, iInfo, -1.0) != -1) {
		return 1;
	}
	return 0;
}
Point3D RayDirectionalLight::transparency(RayIntersectionInfo& iInfo,RayShape* shape,Point3D cLimit){
	Ray3D ray = Ray3D(iInfo.iCoordinate, this->direction.unit().negate());
	Point3D transparency;
	Point3D contribution = Point3D(1,1,1);
	double dist;
	while (contribution[0] > cLimit[0]) {
		Ray3D ray = Ray3D(iInfo.iCoordinate, this->direction.unit().negate());
		dist = shape->intersect(ray, iInfo, -1.0);
		if (dist == -1) {
			break;
		}
		Point3D kT = iInfo.material->transparency;
		iInfo.iCoordinate += 0.0001;
		dist2 = shape->intersect(ray, iInfo, -1.0);
		Point3D contribution = exp(kT*-dist2)
		transparency += contribution
	}
	return transparency

	return Point3D(1,1,1);
}


//////////////////
// OpenGL stuff //
//////////////////
void RayDirectionalLight::drawOpenGL(int index){
}