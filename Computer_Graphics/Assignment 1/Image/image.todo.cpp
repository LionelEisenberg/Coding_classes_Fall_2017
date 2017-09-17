#include "image.h"
#include <stdlib.h>
#include <math.h>

////////////////////////////
// Image processing stuff //
////////////////////////////
Pixel::Pixel(const Pixel32& p)
{
}
Pixel32::Pixel32(const Pixel& p)
{
}

float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

float greyScale(Pixel32 p) {
  const float redWeight = 0.30;
  const float greenWeight = 0.59;
  const float blueWeight = 0.11;
  float luminance = p.r * redWeight + p.g * greenWeight + p.b * blueWeight;
  return luminance;
}

int truncate(float value) {
  if (value > 255) {
    return 255;
  } else if (value < 0) {
    return 0;
  }
  return value;
}

int Image32::AddRandomNoise(const float& noise,Image32& outputImage) const
{
	if (noise >= 0 && noise <= 1) {
		outputImage.setSize(this->width(), this->height());
		for (int i = 0; i < this->width(); i++) {
			for (int j = 0; j < this->height(); j++) {
				outputImage.pixel(i,j) = this->pixel(i,j);
				float rnoise = RandomFloat(0,1)-noise;
				outputImage.pixel(i,j).r *= rnoise;
				outputImage.pixel(i,j).b *= rnoise;
				outputImage.pixel(i,j).g *= rnoise;
			}
		}
		return 1;
	}
	return 0;
}
int Image32::Brighten(const float& brightness,Image32& outputImage) const
{
	outputImage.setSize(this->width(), this->height());
	for (int i = 0; i < this->width(); i++) {
		for (int j = 0; j < this->height(); j++) {
			outputImage.pixel(i,j) = this->pixel(i,j);
			outputImage.pixel(i,j).r = truncate(outputImage.pixel(i,j).r * brightness);
			outputImage.pixel(i,j).b = truncate(outputImage.pixel(i,j).b * brightness);
      outputImage.pixel(i,j).g = truncate(outputImage.pixel(i,j).g * brightness);
		}
	}
	return 1;
}

int Image32::Luminance(Image32& outputImage) const
{
  outputImage.setSize(this->width(), this->height());
	for (int i = 0; i < this->width(); i++) {
		for (int j = 0; j < this->height(); j++) {
      float luminance = greyScale(this->pixel(i,j));
			outputImage.pixel(i,j).r = truncate(luminance);
      outputImage.pixel(i,j).g = truncate(luminance);
      outputImage.pixel(i,j).b = truncate(luminance);
		}
	}
	return 1;
}

int Image32::Contrast(const float& contrast,Image32& outputImage) const
{
  outputImage.setSize(this->width(), this->height());
  float luminance = 0;
	for (int i = 0; i < this->width(); i++) {
		for (int j = 0; j < this->height(); j++) {
      luminance += greyScale(this->pixel(i,j));
		}
	}
  float avgLuminance = luminance / (this->width() * this->height());
  for (int i = 0; i < this->width(); i++) {
	  for (int j = 0; j < this->height(); j++) {
      outputImage.pixel(i,j).r = truncate((this->pixel(i,j).r - avgLuminance) * contrast + avgLuminance);
      outputImage.pixel(i,j).g = truncate((this->pixel(i,j).g - avgLuminance) * contrast + avgLuminance);
      outputImage.pixel(i,j).b = truncate((this->pixel(i,j).b - avgLuminance) * contrast + avgLuminance);
	  }
	}
	return 1;
}

int Image32::Saturate(const float& saturation,Image32& outputImage) const
{
  outputImage.setSize(this->width(), this->height());
  for (int i = 0; i < this->width(); i++) {
	  for (int j = 0; j < this->height(); j++) {
      outputImage.pixel(i,j).r = truncate((this->pixel(i,j).r - greyScale(this->pixel(i,j))) * saturation + greyScale(this->pixel(i,j)));
      outputImage.pixel(i,j).g = truncate((this->pixel(i,j).g - greyScale(this->pixel(i,j))) * saturation + greyScale(this->pixel(i,j)));
      outputImage.pixel(i,j).b = truncate((this->pixel(i,j).b - greyScale(this->pixel(i,j))) * saturation + greyScale(this->pixel(i,j)));
	  }
	}
	return 1;
}

int Image32::Quantize(const int& bits,Image32& outputImage) const
{
  outputImage.setSize(this->width(), this->height());
  for (int i = 0; i < this->width(); i++) {
    for (int j = 0; j < this->height(); j++) {
      //printf("%f\n", floor((this->pixel(i,j).r / 255.0) * pow(2, bits)));
      outputImage.pixel(i,j).r = ceil((this->pixel(i,j).r / 255.0) * (pow(2, bits) + 1) * 255.0 / (pow(2, bits) + 1);
      outputImage.pixel(i,j).g = ceil((this->pixel(i,j).g / 255.0) * (pow(2, bits) + 1) * 255.0 / (pow(2, bits) + 1);
      outputImage.pixel(i,j).b = ceil((this->pixel(i,j).b / 255.0) * (pow(2, bits) + 1) * 255.0 / (pow(2, bits) + 1);
    }
  }
	return 1;
}

int Image32::RandomDither(const int& bits,Image32& outputImage) const
{
	return 0;
}
int Image32::OrderedDither2X2(const int& bits,Image32& outputImage) const
{
	return 0;
}

int Image32::FloydSteinbergDither(const int& bits,Image32& outputImage) const
{
	return 0;
}

int Image32::Blur3X3(Image32& outputImage) const
{
	return 0;
}

int Image32::EdgeDetect3X3(Image32& outputImage) const
{
	return 0;
}
int Image32::ScaleNearest(const float& scaleFactor,Image32& outputImage) const
{
	return 0;
}

int Image32::ScaleBilinear(const float& scaleFactor,Image32& outputImage) const
{
	return 0;
}

int Image32::ScaleGaussian(const float& scaleFactor,Image32& outputImage) const
{
	return 0;
}

int Image32::RotateNearest(const float& angle,Image32& outputImage) const
{
	return 0;
}

int Image32::RotateBilinear(const float& angle,Image32& outputImage) const
{
	return 0;
}

int Image32::RotateGaussian(const float& angle,Image32& outputImage) const
{
	return 0;
}


int Image32::SetAlpha(const Image32& matte)
{
	return 0;
}

int Image32::Composite(const Image32& overlay,Image32& outputImage) const
{
	return 0;
}

int Image32::CrossDissolve(const Image32& source,const Image32& destination,const float& blendWeight,Image32& ouputImage)
{
	return 0;
}
int Image32::Warp(const OrientedLineSegmentPairs& olsp,Image32& outputImage) const
{
	return 0;
}

int Image32::FunFilter(Image32& outputImage) const
{
	return 0;
}
int Image32::Crop(const int& x1,const int& y1,const int& x2,const int& y2,Image32& outputImage) const
{
	return 0;
}


Pixel32 Image32::NearestSample(const float& x,const float& y) const
{
	return Pixel32();
}
Pixel32 Image32::BilinearSample(const float& x,const float& y) const
{
	return Pixel32();
}
Pixel32 Image32::GaussianSample(const float& x,const float& y,const float& variance,const float& radius) const
{
	return Pixel32();
}
