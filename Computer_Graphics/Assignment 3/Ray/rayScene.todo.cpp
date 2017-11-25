#include "rayScene.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <unistd.h>

///////////////////////
// Ray-tracing stuff //
///////////////////////

Point3D RayScene::Reflect(Point3D v,Point3D n){
	return Point3D();
}

int RayScene::Refract(Point3D v,Point3D n,double ir,Point3D& refract){
	return 0;
}

Ray3D RayScene::GetRay(RayCamera* camera,int i,int j,int width,int height){
	return Ray3D();
}

Point3D RayScene::GetColor(Ray3D ray,int rDepth,Point3D cLimit){
	return Point3D();
}

//////////////////
// OpenGL stuff //
//////////////////
void RayMaterial::drawOpenGL(void){
	if (this->tex) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex->openGLHandle);
		printf("makes textures\n");
		this->tex->setUpOpenGL();
	}
	glEnable(GL_BLEND);
	GLfloat alpha = transparent[0] + transparent[1] + transparent[2];
	alpha = alpha / 3;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLfloat dif[] = {diffuse[0], diffuse[1], diffuse[2], 1.0};
	GLfloat spec[] = {specular[0], specular[1], specular[2], 1.0};
	GLfloat emi[] = {emissive[0], emissive[1], emissive[2], 1.0};
	GLfloat shi[] = {this->specularFallOff};

	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	glMaterialfv(GL_FRONT, GL_EMISSION, emi);
	glMaterialfv(GL_FRONT, GL_SHININESS, shi);
	glDisable(GL_BLEND);
}
void RayTexture::setUpOpenGL(void){
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(this->index, &this->openGLHandle);
	glBindTexture(GL_TEXTURE_2D, this->openGLHandle);

	unsigned char tex[this->img->width()][this->img->height()][3];
	for(int i = 0; i < this->img->width(); i++) {
		for(int j = 0; j < this->img->height(); j++) {
			tex[i][j][0] = this->img->pixel(i,j).r;
			tex[i][j][1] = this->img->pixel(i,j).g;
			tex[i][j][2] = this->img->pixel(i,j).b;
		}
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, this->img->width(), this->img->height(), 0, GL_RGB, GL_UNSIGNED_BYTE, tex);
	glBindTexture(GL_TEXTURE_2D, 0);
}
