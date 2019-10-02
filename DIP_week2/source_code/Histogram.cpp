#include "Histogram.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
void Histogram::GetHistogram()
{
	for (int x = 0; x < this->img_in.rows; x++)
		for (int y = 0; y < this->img_in.cols; y++)
		{
			int intensity = *(this->img_in.ptr<uchar>(x) + y);
			this->histogram[intensity]++;
		}

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

Histogram::Histogram(Mat image)
{
	this->img_in = image;
	GetHistogram();
	Mat histImage = Mat(512, 600, CV_8U);

	histImage = Scalar(255);

	Point max = GetMaxHistogram();
	double binW = 2;
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

	double maximum_property = ((double)max.y / (image.cols*image.rows));

	std::stringstream stream;
	stream << std::fixed << std::setprecision(3) << maximum_property;
	std::string myText = stream.str();
	cv::Point myPoint;
	myPoint.x = 0;
	myPoint.y = histImage.rows - (this->histogram[max.x]) * 512 / ((double)max.y) + 20;;
	int myFontFace = 0;
	double myFontScale = 0.42;
	cv::putText(histImage, myText, myPoint, myFontFace, myFontScale, Scalar::all(0));

	maximum_property = (double)max.y / (2 * image.cols*image.rows);
	std::stringstream stream2;
	stream2 << std::fixed << std::setprecision(3) << maximum_property;

	myText = stream2.str();
	myPoint.y = histImage.rows - (this->histogram[max.x]) * 512 / 2 / ((double)max.y) + 20;
	cv::putText(histImage, myText, myPoint, myFontFace, myFontScale, Scalar::all(0));


	myText = "0";
	myPoint.x = binW + 45;
	myPoint.y = histImage.rows - 5;
	cv::putText(histImage, myText, myPoint, myFontFace, myFontScale, Scalar::all(0));

	myText = "255";
	myPoint.x = 255 * binW + 40;
	myPoint.y = histImage.rows - 5;
	cv::putText(histImage, myText, myPoint, myFontFace, myFontScale, Scalar::all(0));

	myText = "63";
	myPoint.x = 63 * binW + 43;
	myPoint.y = histImage.rows - 5;
	cv::putText(histImage, myText, myPoint, myFontFace, myFontScale, Scalar::all(0));
	imshow("histImage", histImage);

	myText = "127";
	myPoint.x = 127 * binW + 40;
	myPoint.y = histImage.rows - 5;
	cv::putText(histImage, myText, myPoint, myFontFace, myFontScale, Scalar::all(0));
	imshow("histImage", histImage);

	myText = "191";
	myPoint.x = 191 * binW + 40;
	myPoint.y = histImage.rows - 5;
	cv::putText(histImage, myText, myPoint, myFontFace, myFontScale, Scalar::all(0));
	this->HistImage = histImage;
}
