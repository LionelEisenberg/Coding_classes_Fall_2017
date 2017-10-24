#include "rayScene.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <typeinfo>
#include <unistd.h>

///////////////////////
// Ray-tracing stuff //
///////////////////////

Point3D truncate(Point3D p) {
  for(int i = 0; i < 3; i++) {
    if (p[i] > 1) {
      p[i] = 1;
    } if (p[i] < 0) {
      p[i] = 0;
    }
  }
  return p;
}

Point3D RayScene::Reflect(Point3D v,Point3D n){
	return Point3D();
}

int RayScene::Refract(Point3D v,Point3D n,double ir,Point3D& refract){
	return 0;
}

Ray3D RayScene::GetRay(RayCamera* camera,int i,int j,int width,int height){
	Ray3D ray;
	float angle = tan(camera->heightAngle / 2);
	float aRatio = camera->aspectRatio;
	Point3D r, l, t, b, x, y;
	Point3D camPos = camera->position;
	Point3D towards = camera->direction;
	Point3D up = camera->up.unit();
	Point3D right = camera->right.unit();
	ray.position = camPos;

	r = right * aRatio * angle;
	l = -right * aRatio * angle;
	t = up * angle;
	b = -up * angle;
	x = (r - l) * (i + 0.5) / width;
	y = (t - b) * (j + 0.5) / height;

	Point3D hit = camPos + towards - t - r + x + y;
	Point3D vector = (hit - camPos);
	ray.direction = vector.unit();
	return ray;
}

Point3D RayScene::GetColor(Ray3D ray,int rDepth,Point3D cLimit){
	RayIntersectionInfo iInfo;
	Point3D diffuse, ambient, emissive, specular;
	double result = this->group->intersect(ray, iInfo, -1.0);
	if (result >= 0) {
		//get diffuse and specular
		for(int i = 0; i < this->lightNum; i++) {
      specular += truncate(this->lights[i]->getSpecular(this->camera->position, iInfo));
      diffuse += truncate(this->lights[i]->getDiffuse(this->camera->position, iInfo));
		}
    //get emissive and ambient
		ambient = iInfo.material->ambient * this->ambient;
		emissive = iInfo.material->emissive;
    Point3D returnValue = truncate(ambient + emissive + diffuse + specular);
		return returnValue;
	} else if (result == -1) {
		return this->background;
	}
	return Point3D(0,0,0);
}

//////////////////
// OpenGL stuff //
//////////////////
void RayMaterial::drawOpenGL(void){
}
void RayTexture::setUpOpenGL(void){
}
