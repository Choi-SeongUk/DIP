#include "IntensityTransform.h"
#include <iostream>
#include <math.h>
IntensityTransform::IntensityTransform(Mat image)
{
	image.convertTo(this->originImg,CV_64F);
}
Mat IntensityTransform::GetImage()
{
	Mat img_out;
	this->originImg.convertTo(img_out, CV_8U);
	return img_out;
}
Mat IntensityTransform::GetImage64()
{
	return originImg;
}IntensityTransform IntensityTransform::Negative(int level)
{
	Mat negative;
	negative = level- originImg;
	return negative;
}

IntensityTransform IntensityTransform::Log(double c)
{
	Mat img_log = Mat(this->originImg.rows, this->originImg.cols, CV_64F);
	for (int x = 0; x < img_log.rows; x++)
		for (int y = 0; y < img_log.cols; y++)
			*(img_log.ptr<double>(x) + y) = c * log(1 + *(this->originImg.ptr<double>(x) + y));
	return IntensityTransform(img_log).IntensityScale(0,255);
}


IntensityTransform IntensityTransform::IntesityMinMax(double min, double max, double l_min, double l_max)
{
	Mat image_sacled;
	this->originImg.copyTo(image_sacled);
	image_sacled = image_sacled - min;
	image_sacled = image_sacled * (l_max-l_min) / (max - min);
	image_sacled = image_sacled + l_min;

	image_sacled.convertTo(image_sacled, CV_64F);
	return image_sacled;
}
IntensityTransform IntensityTransform::IntensityScale()
{
	double min = 0;
	double max = 0;
	for (int x = 0; x < originImg.rows; x++)
		for (int y = 0; y < originImg.cols; y++)
		{
			if (*(originImg.ptr<double>(x) + y) < min)
				min = *(originImg.ptr<double>(x) + y);
			if (*(originImg.ptr<double>(x) + y) > max)
				max = *(originImg.ptr<double>(x) + y);
		}
	std::cout << min << " " << max;
	return IntesityMinMax(min, max, min, max);
}
IntensityTransform IntensityTransform::IntensityScale(double l_min, double l_max)
{
	double min = 0;
	double max = 0;
	for (int x = 0; x < originImg.rows; x++)
		for (int y = 0; y < originImg.cols; y++)
		{
			if (*(originImg.ptr<double>(x) + y) < min)
				min = *(originImg.ptr<double>(x) + y);
			if (*(originImg.ptr<double>(x) + y) > max)
				max = *(originImg.ptr<double>(x) + y);
		}
	std::cout << min << " " << max;
	return IntesityMinMax(min, max, l_min, l_max);
}

IntensityTransform IntensityTransform::GammaRescale(double c, double r)
{
	Mat img_gamma = Mat(this->originImg.rows, this->originImg.cols, CV_64F);

	for (int x = 0; x < originImg.rows; x++)
		for (int y = 0; y < originImg.cols; y++)
		{
			*(img_gamma.ptr<double>(x) + y) = c * pow(*(this->originImg.ptr<double>(x) + y), r);
		}
	
	Mat rescale = IntensityTransform(img_gamma).IntensityScale(this->Min(), this->Max()).GetImage64();
	return rescale;
}

IntensityTransform IntensityTransform::Gamma(double c, double r)
{
	Mat img_gamma = Mat(this->originImg.rows, this->originImg.cols, CV_64F);

	for (int x = 0; x < originImg.rows; x++)
		for (int y = 0; y < originImg.cols; y++)
		{
			*(img_gamma.ptr<double>(x) + y) = c * pow(*(this->originImg.ptr<double>(x) + y),r);
		}
	return img_gamma;
}

double IntensityTransform::Min()
{
	double min = *originImg.ptr<double>(0);
	for (int x = 0; x < originImg.rows; x++)
		for (int y = 0; y < originImg.cols; y++)
			if (*(originImg.ptr<double>(x) + y) < min)
				min = *(originImg.ptr<double>(x) + y);
	return min;
}
double IntensityTransform::Max()
{
	double max = 0;
	for (int x = 0; x < originImg.rows; x++)
		for (int y = 0; y < originImg.cols; y++)
			if (*(originImg.ptr<double>(x) + y) > max)
				max = *(originImg.ptr<double>(x) + y);
	return max;
}

