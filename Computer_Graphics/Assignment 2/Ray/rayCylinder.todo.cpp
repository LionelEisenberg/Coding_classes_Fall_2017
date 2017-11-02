#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "rayScene.h"
#include "rayCylinder.h"


////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RayCylinder::intersect(Ray3D ray,RayIntersectionInfo& iInfo,double mx){
	//intersect with infinit cylinder
	Point3D intersection;
	Point3D vector = Point3D(0,1,0);
	Point3D center = this->center - vector * height / 2; //get center of bottom of the
	Point3D deltaP = ray.position - center;

	//get coefficients for equation
	float a = ray.direction.p[0]*ray.direction.p[0] + ray.direction.p[2]*ray.direction.p[2];
	float b = 2*(ray.position.p[0]*ray.direction.p[0] + ray.position.p[2]*ray.direction.p[2]);
	float c = ray.position.p[0]*ray.position.p[0] + ray.position.p[2]*ray.position.p[2] - this->radius * this->radius;

	//solve for t
	double discriminant = pow(b, 2) - 4 * a * c;
	double tSide, t1, t2;
	//check discriminant
	if (discriminant < 0) {
		return -1;
	} else {
		//check t is in front, find best t out of all posibilities;
		t1 = (-b + sqrt(discriminant)) / (2 * a);
		t2 = (-b - sqrt(discriminant)) / (2 * a);
		Point3D intersection1 = ray.position + ray.direction * t1;
		Point3D intersection2 = ray.position + ray.direction * t2;
		if ((intersection1 - ray.position).length() < (intersection2 - ray.position).length() && t1 > 0.0001) {
			intersection = intersection1;
			return t1;
			if (t1 < 0.0001) {
				return -1;
			}
		} else {
			intersection = intersection2;
			return t2;
			if (t2 < 0.0001) {
				return -1;
			}
		}
		//doing checks on the intersection to validate
		Point3D segment = (intersection - ray.position);
		//if intersectino is higher then the height of the cylinder then it is not valid
		if(intersection.p[1] > (center + vector * this->radius)[1] || intersection.p[1] < (center - vector * this->radius)[1]|| segment.length() == vector.length()) {
			return -1;
		}
		if (mx < 0 || (intersection - ray.position).length() < mx) {
			iInfo.iCoordinate = intersection;
			iInfo.material = this->material;
			iInfo.normal = (intersection - this->center).unit();
		}
		return (intersection - ray.position).length();
	}

	//intesect with top and bottom
}

BoundingBox3D RayCylinder::setBoundingBox(void){
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////
int RayCylinder::drawOpenGL(int materialIndex){
	return -1;
}
