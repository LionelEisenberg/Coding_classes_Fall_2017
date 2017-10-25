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
	return v - n * v.dot(n) * 2;
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

Point3D recursive(Ray3D ray,int rDepth,Point3D cLimit, RayScene* scene) {
  RayIntersectionInfo iInfo;
  Ray3D reflect, refract;
	Point3D diffuse, ambient, emissive, specular;
  int shadow = 1;
	double result = scene->group->intersect(ray, iInfo, -1.0);
  rDepth--;
	if (result >= 0) {
		//get diffuse and specular
    ambient = truncate(iInfo.material->ambient * scene->ambient);
    emissive = truncate(iInfo.material->emissive);
    Point3D kSpec = iInfo.material->specular;
		for(int i = 0; i < scene->lightNum; i++) {
      shadow = 1 - scene->lights[i]->isInShadow(iInfo, scene->group);
      specular += truncate(scene->lights[i]->getSpecular(ray.position, iInfo) * shadow);
      diffuse += truncate(scene->lights[i]->getDiffuse(ray.position, iInfo) * shadow);
		}
    Point3D returnValue = truncate(diffuse + specular);

    reflect.direction = scene->Reflect(ray.direction.unit(), iInfo.normal.unit());
    reflect.position = iInfo.iCoordinate;
    if (rDepth > 0 && (kSpec[0] > cLimit[0] && kSpec[1] > cLimit[1] && kSpec[2] > cLimit[2]) && shadow != 0) {
      returnValue += kSpec * recursive(reflect, rDepth, cLimit/kSpec, scene);
    }
    return truncate(returnValue + ambient + emissive);
  }
  return Point3D();
}

Point3D RayScene::GetColor(Ray3D ray,int rDepth,Point3D cLimit){
	RayIntersectionInfo iInfo;
  Ray3D reflect, refract;
	Point3D diffuse, ambient, emissive, specular;
  int shadow = 1;
	double result = this->group->intersect(ray, iInfo, -1.0);
	if (result >= 0) {
		//get diffuse and specular
    ambient = truncate(iInfo.material->ambient * this->ambient);
    emissive = truncate(iInfo.material->emissive);
    Point3D kSpec = iInfo.material->specular;
		for(int i = 0; i < this->lightNum; i++) {
      shadow = 1 - this->lights[i]->isInShadow(iInfo, this->group);
      specular += truncate(this->lights[i]->getSpecular(ray.position, iInfo) * shadow);
      diffuse += truncate(this->lights[i]->getDiffuse(ray.position, iInfo) * shadow);
		}
    //get emissive and ambient
    Point3D returnValue = truncate(diffuse + specular);

    reflect.direction = this->Reflect(ray.direction.unit(), iInfo.normal.unit());
    reflect.position = iInfo.iCoordinate;
    if (rDepth > 0 && (kSpec[0] > cLimit[0] && kSpec[1] > cLimit[1] && kSpec[2] > cLimit[2]) && shadow != 0) {
      returnValue += kSpec * recursive(reflect, rDepth, cLimit/kSpec, this);
    }

    // refract.direction =
		return truncate(returnValue + ambient + emissive);
	}
	return this->background;
}

//////////////////
// OpenGL stuff //
//////////////////
void RayMaterial::drawOpenGL(void){
}
void RayTexture::setUpOpenGL(void){
}
