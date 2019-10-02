#pragma once
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#define pi 3.14159265359
using namespace cv;
using namespace std;

class HSI_MAT;
class RGB_MAT
{
public:
	RGB_MAT(Mat colorImage);
	RGB_MAT(Mat R, Mat G, Mat B);
	Mat ToImage();
	HSI_MAT ToHSI();
	Mat R;
	Mat G;
	Mat B;

};
/*H:0~2pi(double), s: 0~1(double), I: 0~255 (double)  */
class HSI_MAT
{
public:
	HSI_MAT(Mat H, Mat S, Mat I);
	Mat H;
	Mat S;
	Mat I;
	RGB_MAT ToRGB();
private:
};
