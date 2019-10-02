#pragma once
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
using namespace cv;
class Threshold
{
public:
	Threshold();
	Threshold(Mat image);
	void BasicTh(int i_th);
	void TripleTh(int k1, int k2);
	void Partial_Th();
	void Partial_Th(int rows, int cols);
	void Otsu_tripe();
	void Otsu();
	void GlobalTh(int intialtT, double deltaT);
	void LocalTh(int neigborSize, int a, int b);
	~Threshold();
	Mat currentImg;
};

