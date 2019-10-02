#pragma once
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
using namespace cv;
class Histogram
{
public:
	
	void GetHistogram();
	Point GetMaxHistogram();
	Histogram(Mat image);
	Mat HistImage;
private:
	int histogram[256] = { 0 };
	Mat img_in;
};

