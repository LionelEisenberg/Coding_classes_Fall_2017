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
void printPoint(Point3D p) {
  printf("%f %f %f\n",p[0], p[1], p[2]);
}

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
  // ir = 1
  double cosNI = n.dot(v);
  double a = 1 - pow(ir, 2) * (1 - cosNI * cosNI);
  if (a < 0) {
    return 0;
  } else {
    refract = v * ir - n * (ir * cosNI + sqrt(a));
    return 1;
  }
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
  Point3D shadow;
  double isShadowed = 0;
	double result = scene->group->intersect(ray, iInfo, -1.0);
  rDepth--;
	if (result >= 0) {
		//get diffuse and specular
    ambient = truncate(iInfo.material->ambient * scene->ambient);
    emissive = truncate(iInfo.material->emissive);
    Point3D kSpec = iInfo.material->specular;
    Point3D kTran = iInfo.material->transparent;
		for(int i = 0; i < scene->lightNum; i++) {
      Point3D sTerm = (scene->lights[i]->getSpecular(ray.position, iInfo));
      Point3D dTerm = (scene->lights[i]->getDiffuse(ray.position, iInfo));
      shadow = scene->lights[i]->transparency(iInfo, scene->group, cLimit);
      if (shadow[0] != 1) {
        specular += truncate(sTerm * shadow);
        diffuse += truncate(dTerm * shadow);
        isShadowed = 1;
      } else {
        specular += truncate(sTerm);
        diffuse += truncate(dTerm);
      }
		}
    Point3D returnValue = truncate(diffuse + specular);

    reflect.direction = scene->Reflect(ray.direction.unit(), iInfo.normal.unit());
    reflect.position = iInfo.iCoordinate + reflect.direction * 0.001;
    if (rDepth > 0 && (kSpec[0] > cLimit[0] && kSpec[1] > cLimit[1] && kSpec[2] > cLimit[2]) && isShadowed != 1) {
      returnValue += kSpec * recursive(reflect, rDepth, cLimit/kSpec, scene);
    }

    int isRefract = scene->Refract(ray.direction.unit(), iInfo.normal.unit(), iInfo.material->refind, refract.direction);
    refract.position = iInfo.iCoordinate + refract.direction * 0.001;
    refract.direction = refract.direction.unit();
    if (rDepth > 0 && (kTran[0] > cLimit[0] && kTran[1] > cLimit[1] && kTran[2] > cLimit[2]) && isShadowed != 1 && isRefract == 1) {
      returnValue += kTran * recursive(refract, rDepth, cLimit/kTran, scene);
    }
    return truncate(returnValue + ambient + emissive);
  }
  return Point3D();
}

Point3D RayScene::GetColor(Ray3D ray,int rDepth,Point3D cLimit){
	RayIntersectionInfo iInfo;
  Ray3D reflect, refract;
	Point3D diffuse, ambient, emissive, specular, shadow;
  double isShadowed = 0;
	double result = this->group->intersect(ray, iInfo, -1.0);
	if (result >= 0) {
		//get diffuse and specular
    ambient = truncate(iInfo.material->ambient * this->ambient);
    emissive = truncate(iInfo.material->emissive);
    Point3D kSpec = iInfo.material->specular;
    Point3D kTran = iInfo.material->transparent;
		for(int i = 0; i < this->lightNum; i++) {
      Point3D sTerm = truncate(this->lights[i]->getSpecular(ray.position, iInfo));
      Point3D dTerm = truncate(this->lights[i]->getDiffuse(ray.position, iInfo));
      shadow = this->lights[i]->transparency(iInfo, this->group, cLimit);
      if (shadow[0] != 1) {
        specular += truncate(sTerm * shadow);
        diffuse += truncate(dTerm * shadow);
        isShadowed = 1;
      } else {
        specular += truncate(sTerm);
        diffuse += truncate(dTerm);
      }
		}
    //get emissive and ambient
    Point3D returnValue = truncate(diffuse + specular);
    // printf("%f\n", isShadowed);
    reflect.direction = this->Reflect(ray.direction.unit(), iInfo.normal.unit());
    reflect.position = iInfo.iCoordinate + reflect.direction * 0.001;
    if (rDepth > 0 && (kSpec[0] > cLimit[0] && kSpec[1] > cLimit[1] && kSpec[2] > cLimit[2]) && isShadowed != 1) {
      returnValue += kSpec * recursive(reflect, rDepth, cLimit/kSpec, this);
    }

    int isRefract = this->Refract(ray.direction.unit(), iInfo.normal.unit(), iInfo.material->refind, refract.direction);
    refract.position = iInfo.iCoordinate + refract.direction * 0.001;
    refract.direction = refract.direction.unit();
    if (rDepth > 0 && (kTran[0] > cLimit[0] && kTran[1] > cLimit[1] && kTran[2] > cLimit[2]) && isShadowed != 1 && isRefract == 1) {
      returnValue += kTran * recursive(refract, rDepth, cLimit/kTran, this);
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
