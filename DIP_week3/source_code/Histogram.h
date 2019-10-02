#pragma once
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
using namespace cv;
using namespace std;
class Histogram
{
public:
	std::array<int,256> GetHistogram();
	std::array<double,256> GetNormalizedHistogram();
	double GetCumulativeSum(int start, int end);
	double GetMeanIntensity(int start, int end);
	std::array<double, 256> GetCumulativeMean();
	double GetCumulativeMean(int start, int end);
	Point GetMaxHistogram();
	Histogram();
	Histogram(Mat image);
	Mat GetHistImg();
	
	double binW = 2;

private:
	std::array<int, 256> histogram;
	std::array<double, 256> histogram_nor;
	Mat img_in;
};

