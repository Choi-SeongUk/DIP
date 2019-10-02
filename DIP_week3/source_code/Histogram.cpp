#include "Histogram.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>

std::array<int, 256> Histogram::GetHistogram()
{
	std::array<int, 256> his = { 0 };
	for (int x = 0; x < this->img_in.rows; x++)
		for (int y = 0; y < this->img_in.cols; y++)
		{
			int intensity = *(this->img_in.ptr<uchar>(x) + y);
			his[intensity] = his[intensity]+1;
		}
	return his;
}
std::array<double, 256> Histogram::GetNormalizedHistogram()
{
	array<double, 256> histogram_nor;
	for (int i = 0; i<256; i++)
		histogram_nor[i] = ((double)histogram[i])/(img_in.rows*img_in.cols);
	return histogram_nor;
}

double Histogram::GetCumulativeSum(int start, int end)
{
	double sum = 0;
	for (int i = start; i <= end; i++)
		sum += this->histogram_nor[i];
	return sum;
}
double Histogram::GetMeanIntensity(int start, int end)
{
	double sum_p = 0;
	double sum_i = 0;
	for (int i = start; i <= end; i++)
	{
		sum_p += this->histogram[i];
		sum_i += this->histogram[i] * i;
	}
	if (sum_p == 0)
		return 0;
	double mean = sum_i / sum_p;
	return mean;
}
std::array<double, 256> Histogram::GetCumulativeMean()
{
	array<double, 256> histogram_nor = GetNormalizedHistogram();
	array<double, 256> cumulativeMean;
	for (int k = 0; k < 256; k++)
	{
		double mean = 0;
		for (int i = 0; i <= k; i++)
		{
			mean += this->histogram_nor[i] * i;
		}
		cumulativeMean[k] = mean;
	}
	return cumulativeMean;
}
double Histogram::GetCumulativeMean(int start, int end)
{
	double mean = 0;
	for (int i = start; i <= end; i++)
		mean += this->histogram_nor[i] * i;

	return mean;
}

Point Histogram::GetMaxHistogram()
{
	int max = 0;
	int index = 0;
	for (int i = 0; i < 256; i++)
	{
		if (max < this->histogram[i])
		{
			max = this->histogram[i];
			index = i;
		}
	}
	return Point(index, max);
}
Histogram::Histogram()
{

}

Histogram::Histogram(Mat image)
{
	this->img_in = image;
	this->histogram = GetHistogram();
	this->histogram_nor = GetNormalizedHistogram();
}

Mat Histogram::GetHistImg()
{
	Mat histImage = Mat(512, 600, CV_8U);
	histImage = Scalar(255);

	Point max = GetMaxHistogram();
	binW = 2;
	int x1, y1, x2, y2;
	for (int i = 0; i < 256; i++)
	{
		double height = (this->histogram[i]);
		height = 512 * height / (max.y);
		x1 = i * binW + 50;
		y1 = histImage.rows - 20 - (this->histogram[i])*(512 - 40) / ((double)max.y);
		x2 = x1 + binW;
		y2 = histImage.rows - 20;
		if (y2 > y1)
			rectangle(histImage, Point(x1, y1), Point(x2, y2), 5, -1);
	}

	double maximum_property = ((double)max.y / (this->img_in.cols*this->img_in.rows));

	std::stringstream stream;
	stream << std::fixed << std::setprecision(3) << maximum_property;
	std::string myText = stream.str();
	cv::Point myPoint;
	myPoint.x = 0;
	myPoint.y = histImage.rows - (this->histogram[max.x]) * 512 / ((double)max.y) + 20;;
	int myFontFace = 0;
	double myFontScale = 0.42;
	cv::putText(histImage, myText, myPoint, myFontFace, myFontScale, Scalar::all(0));

	maximum_property = (double)max.y / (2 * this->img_in.cols*this->img_in.rows);
	std::stringstream stream2;
	stream2 << std::fixed << std::setprecision(3) << maximum_property;

	myText = stream2.str();
	myPoint.y = histImage.rows - (this->histogram[max.x]) * 512 / 2 / ((double)max.y) + 20;
	cv::putText(histImage, myText, myPoint, myFontFace, myFontScale, Scalar::all(0));


	myText = "0";
	myPoint.x = binW + 43;
	myPoint.y = histImage.rows - 5;
	cv::putText(histImage, myText, myPoint, myFontFace, myFontScale, Scalar::all(0));

	myText = "255";
	myPoint.x = 255 * binW + 39;
	myPoint.y = histImage.rows - 5;
	cv::putText(histImage, myText, myPoint, myFontFace, myFontScale, Scalar::all(0));

	myText = "63";
	myPoint.x = 63 * binW + 41;
	myPoint.y = histImage.rows - 5;
	cv::putText(histImage, myText, myPoint, myFontFace, myFontScale, Scalar::all(0));

	myText = "127";
	myPoint.x = 127 * binW + 38;
	myPoint.y = histImage.rows - 5;
	cv::putText(histImage, myText, myPoint, myFontFace, myFontScale, Scalar::all(0));

	myText = "191";
	myPoint.x = 191 * binW + 38;
	myPoint.y = histImage.rows - 5;
	cv::putText(histImage, myText, myPoint, myFontFace, myFontScale, Scalar::all(0));
	return histImage;
}
