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
			outputImage.pixel(i,j).r *= brightness;
			outputImage.pixel(i,j).b *= brightness;
			outputImage.pixel(i,j).g *= brightness;
		}
	}
	return 1;
}

int Image32::Luminance(Image32& outputImage) const
{
	return 0;
}

int Image32::Contrast(const float& contrast,Image32& outputImage) const
{
	return 0;
}

int Image32::Saturate(const float& saturation,Image32& outputImage) const
{
	return 0;
}

int Image32::Quantize(const int& bits,Image32& outputImage) const
{
	return 0;
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
