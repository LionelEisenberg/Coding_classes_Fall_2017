#include "image.h"
#include <stdlib.h>
#include <math.h>
#include <algorithm>

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

//adds a random float between -255 and 255 scaled with the noise input to the image.
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
            sumR += 128/(-1/8.0);
            sumB += 128/(-1/8.0);
            sumG += 128/(-1/8.0);
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
      outputImage.pixel(i,j) = this->NearestSample(i / scaleFactor, j / scaleFactor);
    }
  }
	return 1;
}

int Image32::ScaleBilinear(const float& scaleFactor,Image32& outputImage) const
{
  outputImage.setSize(this->width() * scaleFactor, this->height() * scaleFactor);
  for (int i = 0; i < outputImage.width(); i++) {
    for (int j = 0; j < outputImage.height(); j++) {
      outputImage.pixel(i,j) = this->BilinearSample(i / scaleFactor, j / scaleFactor);
    }
  }
	return 1;
}

int Image32::ScaleGaussian(const float& scaleFactor,Image32& outputImage) const
{
  const float variance = 1.0/scaleFactor;
  const int radius = 1;
  outputImage.setSize(this->width() * scaleFactor, this->height() * scaleFactor);
  for (int i = 0; i < outputImage.width(); i++) {
    for (int j = 0; j < outputImage.height(); j++) {
      outputImage.pixel(i,j) = this->GaussianSample(i / scaleFactor, j / scaleFactor, variance, radius);
    }
  }
	return 1;
}



int Image32::RotateNearest(const float& angle,Image32& outputImage) const
{
  int height = this->height(); int width = this->width();
  float radians = - angle * (PI / 180);
  double x0 = 0;
  double y0 = 0;
  double x1 = width * cos(radians);
  double y1 = width * sin(radians);
  double x2 = width * cos(radians) - height * sin(radians);
  double y2 = height * cos(radians) + width * sin(radians);
  double x3 = height * sin(radians) * -1;
  double y3 = height * cos(radians);
  double Xarr[4] = {x0, x1, x2, x3};
  double Yarr[4] = {y0, y1, y2, y3};
  double Xminimum = *std::min_element(Xarr, Xarr + 4);
  double Yminimum = *std::min_element(Yarr, Yarr + 4);

  // Formula taken from: https://stackoverflow.com/questions/3231176/how-to-get-size-of-a-rotated-rectangle
  int newHeight = abs(width * sin(radians)) + abs(height * cos(radians)) + 1;
  int newWidth = abs(width * cos(radians)) + abs(height * sin(radians)) + 1;
  outputImage.setSize(newWidth, newHeight);

  for (int j = 0; j < outputImage.height(); j++) {
    for (int i = 0; i < outputImage.width(); i++) {
			outputImage(i, j) = this->NearestSample((i + Xminimum) * cos(radians) + (j + Yminimum) * sin(radians), (j + Yminimum) * cos(radians) - (i + Xminimum) * sin(radians));
		}
  }
	return 1;
}

int Image32::RotateBilinear(const float& angle,Image32& outputImage) const
{
  int height = this->height(); int width = this->width();
  float radians = - angle * (PI / 180);
  double x0 = 0;
  double y0 = 0;
  double x1 = width * cos(radians);
  double y1 = width * sin(radians);
  double x2 = width * cos(radians) - height * sin(radians);
  double y2 = height * cos(radians) + width * sin(radians);
  double x3 = height * sin(radians) * -1;
  double y3 = height * cos(radians);
  double Xarr[4] = {x0, x1, x2, x3};
  double Yarr[4] = {y0, y1, y2, y3};
  double Xminimum = *std::min_element(Xarr, Xarr + 4);
  double Yminimum = *std::min_element(Yarr, Yarr + 4);

  // Formula taken from: https://stackoverflow.com/questions/3231176/how-to-get-size-of-a-rotated-rectangle
  int newHeight = abs(width * sin(radians)) + abs(height * cos(radians)) + 1;
  int newWidth = abs(width * cos(radians)) + abs(height * sin(radians)) + 1;
  outputImage.setSize(newWidth, newHeight);

  for (int j = 0; j < outputImage.height(); j++) {
    for (int i = 0; i < outputImage.width(); i++) {
			outputImage(i, j) = this->BilinearSample((i + Xminimum) * cos(radians) + (j + Yminimum) * sin(radians), (j + Yminimum) * cos(radians) - (i + Xminimum) * sin(radians));
		}
  }
	return 1;
}

int Image32::RotateGaussian(const float& angle,Image32& outputImage) const
{
  const float variance = 1.0;
  const int radius = 1;
  int height = this->height(); int width = this->width();
  float radians = - angle * (PI / 180);
  double x0 = 0;
  double y0 = 0;
  double x1 = width * cos(radians);
  double y1 = width * sin(radians);
  double x2 = width * cos(radians) - height * sin(radians);
  double y2 = height * cos(radians) + width * sin(radians);
  double x3 = height * sin(radians) * -1;
  double y3 = height * cos(radians);
  double Xarr[4] = {x0, x1, x2, x3};
  double Yarr[4] = {y0, y1, y2, y3};
  double Xminimum = *std::min_element(Xarr, Xarr + 4);
  double Yminimum = *std::min_element(Yarr, Yarr + 4);

  // Formula taken from: https://stackoverflow.com/questions/3231176/how-to-get-size-of-a-rotated-rectangle
  int newHeight = abs(width * sin(radians)) + abs(height * cos(radians)) + 1;
  int newWidth = abs(width * cos(radians)) + abs(height * sin(radians)) + 1;
  outputImage.setSize(newWidth, newHeight);

  for (int j = 0; j < outputImage.height(); j++) {
    for (int i = 0; i < outputImage.width(); i++) {
			outputImage(i, j) = this->GaussianSample((i + Xminimum) * cos(radians) + (j + Yminimum) * sin(radians), (j + Yminimum) * cos(radians) - (i + Xminimum) * sin(radians), variance, radius);
		}
  }
	return 1;
}


int Image32::SetAlpha(const Image32& matte)
{
  for (int i = 0; i < matte.width(); i++) {
    for (int j = 0; j < matte.height(); j++) {
      if (matte.pixel(i,j).r >= 220 && matte.pixel(i,j).g <= 35 && matte.pixel(i,j).b <= 35) {
        this->pixel(i,j).a = 0;
      } else {
        this->pixel(i,j).a = 1;
      }
    }
  }
}

int Image32::Composite(const Image32& overlay,Image32& outputImage) const
{
  outputImage.setSize(this->width(), this->height());
  for (int i = 0; i < this->width(); i++) {
    for (int j = 0; j < this->height(); j++) {
      if (overlay.pixel(i,j).a == 1) {
        outputImage.pixel(i,j) = overlay.pixel(i,j);
      } else {
        outputImage.pixel(i,j) = this->pixel(i,j);
      }
    }
  }
	return 1;
}

int Image32::CrossDissolve(const Image32& source,const Image32& destination,const float& blendWeight,Image32& ouputImage)
{
	return 0;
}
int Image32::Warp(const OrientedLineSegmentPairs& olsp,Image32& outputImage) const
{
	return 0;
}

//algorithm take from http://introcs.cs.princeton.edu
int Image32::FunFilter(Image32& outputImage) const
{
  float swirlStrength = 1.0;
  outputImage.setSize(this->width(), this->height());
  int x0 = outputImage.width() / 2;
  int y0 = outputImage.height() / 2;
  for (int i = 0; i < outputImage.width(); i++) {
    for (int j = 0; j < outputImage.height(); j++) {
      float distx = i - x0;
      float disty = j - y0;
      float r = sqrt(pow(distx, 2) + pow(disty, 2));
      float rangle = PI / 256 * r * swirlStrength;
      outputImage.pixel(i,j) = this->NearestSample(distx * cos(rangle) - disty * sin(rangle) + x0, distx * sin(rangle) + disty * cos(rangle) + y0);
    }
  }
	return 1;
}

int Image32::Crop(const int& x1,const int& y1,const int& x2,const int& y2,Image32& outputImage) const
{
  outputImage.setSize(x2-x1, y2-y1);
  for (int i = 0; i < outputImage.width(); i++) {
    for (int j = 0; j < outputImage.height(); j++) {
      if (i + x1 >= this->width() || i + x1 < 0 || j + y1 >= this->height() || j + y1 < 0) {
        Pixel32 dark;
        outputImage.pixel(i,j) = dark;
      } else {
        outputImage.pixel(i,j) = this->pixel(i + x1, j + y1);
      }
    }
  }
	return 1;
}


Pixel32 Image32::NearestSample(const float& x,const float& y) const
{
  int ix = floor(x + 0.5);
  int iy = floor(y + 0.5);
  if (ix < 0 || ix >= this->width() || iy < 0 || iy >= this->height()) {
    Pixel32 dark;
    return dark;
  }
  return this->pixel(ix, iy);
}

Pixel32 Image32::BilinearSample(const float& x,const float& y) const
{
  int x1, x2, y1, y2;
  float dy, dx;
  x1 = floor(x);
  y1 = floor(y);
  x2 = x1 + 1;
  y2 = y1 + 1;
  dx = x - x1;
  dy = y - y1;
  Pixel32 bottomInterpolation, topInterpollation, sourcePixel;

  if (x >= this->width() || x < 0 || y >= this->width() || y < 0) {
    return Pixel32();
  }

  if (x2 >= this->width()) {
    bottomInterpolation.r = this->pixel(x1,y1).r * (1 - dx);
    bottomInterpolation.g = this->pixel(x1,y1).g * (1 - dx);
    bottomInterpolation.b = this->pixel(x1,y1).b * (1 - dx);

    if(y2 < this->height()) {
      topInterpollation.r = this->pixel(x1,y2).r * (1 - dx);
      topInterpollation.g = this->pixel(x1,y2).g * (1 - dx);
      topInterpollation.b = this->pixel(x1,y2).b * (1 - dx);
    }
  } else {
    bottomInterpolation.r = this->pixel(x1,y1).r * (1 - dx) + this->pixel(x2,y1).r * dx;
    bottomInterpolation.g = this->pixel(x1,y1).g * (1 - dx) + this->pixel(x2,y1).g * dx;
    bottomInterpolation.b = this->pixel(x1,y1).b * (1 - dx) + this->pixel(x2,y1).b * dx;

    if(y2 < this->height()) {
      topInterpollation.r = this->pixel(x1,y2).r * (1 - dx) + this->pixel(x2,y2).r * dx;
      topInterpollation.g = this->pixel(x1,y2).g * (1 - dx) + this->pixel(x2,y2).g * dx;
      topInterpollation.b = this->pixel(x1,y2).b * (1 - dx) + this->pixel(x2,y2).b * dx;
    }
  }
  sourcePixel.r = truncate(bottomInterpolation.r * (1 - dy) + topInterpollation.r * dy);
  sourcePixel.g = truncate(bottomInterpolation.g * (1 - dy) + topInterpollation.g * dy);
  sourcePixel.b = truncate(bottomInterpolation.b * (1 - dy) + topInterpollation.b * dy);

	return sourcePixel;
}

Pixel32 Image32::GaussianSample(const float& x,const float& y,const float& variance,const float& radius) const
{
  if (x >= this->width() || x < 0 || y >= this->width() || y < 0) {
    return Pixel32();
  }

  int xLowerBound = floor(x) - radius;
  int yLowerBound = floor(y) - radius;
  int xUpperBound = ceil(x) + radius;
  int yUpperBound = ceil(y) + radius;
  Pixel32 sampledPixel;

  float normalize = 0.0;
  float mask[xUpperBound - xLowerBound][yUpperBound - yLowerBound];

  for (int i = xLowerBound; i < xUpperBound; i++) {
    for (int j = yLowerBound; j < yUpperBound; j++) {
        float di = abs(x - i);
        float dj = abs(y - j);
        mask[i - xLowerBound][j - yLowerBound] = exp(-(pow(di, 2) + pow(dj, 2))/(2*pow(variance, 2)));
        normalize += mask[i - xLowerBound][j - yLowerBound];
    }
  }

  for (int i = xLowerBound; i < xUpperBound; i++) {
    for (int j = yLowerBound; j < yUpperBound; j++) {
      if (i < 0 || i >= this->width() || j < 0 || j >= this->height()) {
        continue;
      } else {
        sampledPixel.r = truncate(sampledPixel.r + this->pixel(i,j).r * mask[i - xLowerBound][j - yLowerBound] / normalize);
        sampledPixel.g = truncate(sampledPixel.g + this->pixel(i,j).g * mask[i - xLowerBound][j - yLowerBound] / normalize);
        sampledPixel.b = truncate(sampledPixel.b + this->pixel(i,j).b * mask[i - xLowerBound][j - yLowerBound] / normalize);
      }
    }
  }
	return sampledPixel;
}
