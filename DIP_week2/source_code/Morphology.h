#pragma once
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
using namespace cv;
class Morphology
{
public:
	void Erosion(Mat mask);
	void Dilation(Mat mask);
	Morphology(Mat in);
	Mat currentImg;
private:
	int high = 250;
	int low = 10;
};

