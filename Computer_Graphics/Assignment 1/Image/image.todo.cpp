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
		outputImage.setSize(this->width(), this->height());
		for (int i = 0; i < this->width(); i++) {
			for (int j = 0; j < this->height(); j++) {
				outputImage.pixel(i,j) = this->pixel(i,j);
				float rnoise1 = RandomFloat(-255,255)*noise;
        float rnoise2 = RandomFloat(-255,255)*noise;
        float rnoise3 = RandomFloat(-255,255)*noise;
				outputImage.pixel(i,j).r = truncate(outputImage.pixel(i,j).r + rnoise1);
				outputImage.pixel(i,j).b = truncate(outputImage.pixel(i,j).b + rnoise2);
				outputImage.pixel(i,j).g = truncate(outputImage.pixel(i,j).g + rnoise3);
			}
		}
		return 1;
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
        outputImage.pixel(i,j).r = truncate(floor((this->pixel(i,j).r / 255.0) * pow(2, bits)) / (pow(2, bits)-1) * 255.0);
        outputImage.pixel(i,j).g = truncate(floor((this->pixel(i,j).g / 255.0) * pow(2, bits)) / (pow(2, bits)-1) * 255.0);
        outputImage.pixel(i,j).b = truncate(floor((this->pixel(i,j).b / 255.0) * pow(2, bits)) / (pow(2, bits)-1) * 255.0);
      }
    }
	return 1;
}

int Image32::RandomDither(const int& bits,Image32& outputImage) const
{
  outputImage.setSize(this->width(), this->height());
  for (int i = 0; i < this->width(); i++) {
    for (int j = 0; j < this->height(); j++) {
        float rnoise1 = RandomFloat(-1,1) / (float) pow(2, bits);
        float rnoise2 = RandomFloat(-1,1) / (float) pow(2, bits);
        float rnoise3 = RandomFloat(-1,1) / (float) pow(2, bits);
        outputImage.pixel(i,j).r = truncate(floor(((this->pixel(i,j).r / 255.0) + rnoise1) * pow(2, bits)) / (pow(2, bits)-1) * 255.0);
        outputImage.pixel(i,j).g = truncate(floor(((this->pixel(i,j).g / 255.0) + rnoise2) * pow(2, bits)) / (pow(2, bits)-1) * 255.0);
        outputImage.pixel(i,j).b = truncate(floor(((this->pixel(i,j).b / 255.0) + rnoise3) * pow(2, bits)) / (pow(2, bits)-1) * 255.0);
      }
    }
  return 1;
}

int Image32::OrderedDither2X2(const int& bits,Image32& outputImage) const
{
  const int n = 2;
  outputImage.setSize(this->width(), this->height());
  for (int i = 0; i < this->width(); i++) {
    for (int j = 0; j < this->height(); j++) {
        int ditherWeight;
        if (i % n == 0 && j % n == 0) {
          ditherWeight = 1;
        } else if (i % n != 0 && j % n != 0) {
          ditherWeight = 2;
        } else if (i % n == 0 && j % n != 0) {
          ditherWeight = 3;
        } else if (i % n != 0 && j % n == 0) {
          ditherWeight = 4;
        }

        float cR = (this->pixel(i,j).r / 255.0) * (pow(2, bits) - 1);
        float cG = (this->pixel(i,j).g / 255.0) * (pow(2, bits) - 1);
        float cB = (this->pixel(i,j).b / 255.0) * (pow(2, bits) - 1);

        float eR = cR - floor(cR);
        float eG = cG - floor(cG);
        float eB = cB - floor(cB);

        if (eR > ditherWeight / (pow(n,2) + 1)) {
          outputImage.pixel(i,j).r = truncate(ceil(cR) * 255 / (pow(2, bits) - 1));
        } else {
          outputImage.pixel(i,j).r = truncate(floor(cR) * 255 / (pow(2, bits) - 1));
        } if (eG > ditherWeight / (pow(n,2) + 1)) {
          outputImage.pixel(i,j).g = truncate(ceil(cG) * 255 / (pow(2, bits) - 1));
        } else {
          outputImage.pixel(i,j).g = truncate(floor(cG) * 255 / (pow(2, bits) - 1));
        } if (eB > ditherWeight / (pow(n,2) + 1)) {
          outputImage.pixel(i,j).b = truncate(ceil(cB) * 255 / (pow(2, bits) - 1));
        } else {
          outputImage.pixel(i,j).b = truncate(floor(cB) * 255 / (pow(2, bits) - 1));
        }
      }
    }
  return 1;
}

int Image32::FloydSteinbergDither(const int& bits,Image32& outputImage) const
{
  const float alpha = 7/16.0;
  const float beta = 3/16.0;
  const float gamma = 5/16.0;
  const float delta = 1/16.0;
  outputImage.setSize(this->width(), this->height());
  Image32 quantized;
  quantized.setSize(this->width(), this->height());
  for (int i = 0; i < this->width(); i++) {
    for (int j = 0; j < this->height(); j++) {
      quantized.pixel(i,j) = this->pixel(i,j);
    }
  }
  for (int j = 0; j < this->height(); j++) {
    for (int i = 0; i < this->width(); i++) {
        outputImage.pixel(i,j).r = truncate(floor((quantized.pixel(i,j).r / 255.0) * pow(2, bits)) / (pow(2, bits)-1) * 255.0);
        outputImage.pixel(i,j).g = truncate(floor((quantized.pixel(i,j).g / 255.0) * pow(2, bits)) / (pow(2, bits)-1) * 255.0);
        outputImage.pixel(i,j).b = truncate(floor((quantized.pixel(i,j).b / 255.0) * pow(2, bits)) / (pow(2, bits)-1) * 255.0);

        float errorR = quantized.pixel(i,j).r - outputImage.pixel(i,j).r;
        float errorG = quantized.pixel(i,j).g - outputImage.pixel(i,j).g;
        float errorB = quantized.pixel(i,j).b - outputImage.pixel(i,j).b;

        if (i + 1 != this->width()) {
          if (j + 1 != this->height()) {
            quantized.pixel(i + 1,j + 1).r = truncate(quantized.pixel(i + 1,j + 1).r + errorR * delta);
            quantized.pixel(i + 1,j + 1).g = truncate(quantized.pixel(i + 1,j + 1).g + errorG * delta);
            quantized.pixel(i + 1,j + 1).b = truncate(quantized.pixel(i + 1,j + 1).b + errorB * delta);
          }
          quantized.pixel(i + 1,j).r = truncate(quantized.pixel(i + 1,j).r + errorR * alpha);
          quantized.pixel(i + 1,j).g = truncate(quantized.pixel(i + 1,j).g + errorG * alpha);
          quantized.pixel(i + 1,j).b = truncate(quantized.pixel(i + 1,j).b + errorB * alpha);
        }

        if (j + 1 != this->height()) {
          if (i - 1 >= 0) {
            quantized.pixel(i - 1,j + 1).r = truncate(quantized.pixel(i - 1,j + 1).r + errorR * beta);
            quantized.pixel(i - 1,j + 1).g = truncate(quantized.pixel(i - 1,j + 1).g + errorG * beta);
            quantized.pixel(i - 1,j + 1).b = truncate(quantized.pixel(i - 1,j + 1).b + errorB * beta);
          }

          quantized.pixel(i, j + 1).r = truncate(quantized.pixel(i,j + 1).r + errorR * gamma);
          quantized.pixel(i, j + 1).g = truncate(quantized.pixel(i,j + 1).g + errorG * gamma);
          quantized.pixel(i, j + 1).b = truncate(quantized.pixel(i,j + 1).b + errorB * gamma);
        }
      }
    }
	return 1;
}

int Image32::Blur3X3(Image32& outputImage) const
{
  outputImage.setSize(this->width(), this->height());
  float blurMatrix[3][3] = {{1/16.0, 2/16.0, 1/16.0}, {2/16.0, 4/16.0, 2/16.0}, {1/16.0, 2/16.0, 1/16.0}};
  for (int i = 0; i < this->width(); i++) {
	  for (int j = 0; j < this->height(); j++) {
      float sumR = 0;
      float sumB = 0;
      float sumG = 0;
      for (int k = i - 1; k <= i + 1; k++) {
        for (int l = j - 1; l <= j + 1; l++) {
          if (k <= 0|| k >= this->width() || l <= 0 || l >= this->height()) {
            continue;
          } else {
            sumR += this->pixel(k,l).r * blurMatrix[k-(i-1)][l-(j-1)];
            sumB += this->pixel(k,l).b * blurMatrix[k-(i-1)][l-(j-1)];
            sumG += this->pixel(k,l).g * blurMatrix[k-(i-1)][l-(j-1)];
          }
        }
      }
      outputImage.pixel(i,j).r = truncate(sumR);
      outputImage.pixel(i,j).g = truncate(sumG);
      outputImage.pixel(i,j).b = truncate(sumB);
	  }
	}
	return 1;
}

int Image32::EdgeDetect3X3(Image32& outputImage) const
{
  outputImage.setSize(this->width(), this->height());
  float edgeMatrix[3][3] = {{-1/8.0, -1/8.0, -1/8.0}, {-1/8.0, 1, -1/8.0}, {-1/8.0, -1/8.0, -1/8.0}};
  for (int i = 0; i < this->width(); i++) {
    for (int j = 0; j < this->height(); j++) {
      float sumR = 0;
      float sumB = 0;
      float sumG = 0;
      for (int k = i - 1; k <= i + 1; k++) {
        for (int l = j - 1; l <= j + 1; l++) {
          if (k <= 0|| k >= this->width() || l <= 0 || l >= this->height()) {
            continue;
          } else {
            sumR += this->pixel(k,l).r * edgeMatrix[k-(i-1)][l-(j-1)];
            sumB += this->pixel(k,l).b * edgeMatrix[k-(i-1)][l-(j-1)];
            sumG += this->pixel(k,l).g * edgeMatrix[k-(i-1)][l-(j-1)];
          }
        }
      }
      outputImage.pixel(i,j).r = truncate(sumR * 5);
      outputImage.pixel(i,j).g = truncate(sumG * 5);
      outputImage.pixel(i,j).b = truncate(sumB * 5);
    }
  }
  return 1;
}

int Image32::ScaleNearest(const float& scaleFactor,Image32& outputImage) const
{
  outputImage.setSize(this->width() * scaleFactor, this->height() * scaleFactor);
  for (int i = 0; i < outputImage.width(); i++) {
    for (int j = 0; j < outputImage.height(); j++) {
      outputImage.pixel(i,j) = NearestSample(i / scaleFactor, j / scaleFactor);
    }
  }
	return 1;
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
  outputImage.setSize(x2-x1, y2-y1);
  for (int i = 0; i < outputImage.width(); i++) {
    for (int j = 0; j < outputImage.height(); j++) {
      outputImage.pixel(i,j) = this->pixel(i + x1, j + y1);
    }
  }
	return 1;
}


Pixel32 Image32::NearestSample(const float& x,const float& y) const
{
  int ix = floor(x + 0.5);
  int iy = floor(y + 0.5);
  return this->pixel(ix, iy);
}

Pixel32 Image32::BilinearSample(const float& x,const float& y) const
{
	return Pixel32();
}
Pixel32 Image32::GaussianSample(const float& x,const float& y,const float& variance,const float& radius) const
{
	return Pixel32();
}
