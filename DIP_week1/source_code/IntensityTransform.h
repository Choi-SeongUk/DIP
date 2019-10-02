#pragma once
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace cv;
using namespace std;
class IntensityTransform
{
public:
	IntensityTransform(Mat image);
	Mat GetImage();
	IntensityTransform Negative(int level);
	Mat GetImage64();
	IntensityTransform Log(double c);
	
	IntensityTransform IntesityMinMax(double min, double max, double l_min, double l_max);
	IntensityTransform IntensityScale();
	IntensityTransform IntensityScale(double l_min, double l_max);

	IntensityTransform GammaRescale(double c, double r);

	IntensityTransform Gamma(double c, double r);

	double Min();

	double Max();
	Mat originImg;

private:
	
	
};

