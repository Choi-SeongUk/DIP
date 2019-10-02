#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <math.h>
#include <time.h>
#include "Affine.h"
#include "SpatialFilter.h"
#include "IntensityTransform.h"
#include "ColorTransform.h"
#include "Histogram.h"
#include "Morphology.h"
#define pi 3.14159265358979323846264338327950288419716939937510582097494459230781

using namespace cv;
using namespace std;

int main()
{
	string img_name = "test_text";
	cv::Mat image = cv::imread("C:\\DIP\\IMAGES\\week2\\" + img_name + ".png", cv::IMREAD_GRAYSCALE);
	Morphology mor = Morphology(image);
	int ker_size = 11;
	Mat mask = (Mat_<uchar>(3, 3) << 255, 255, 255, 255, 255, 255, 255, 255, 255);
	mor.Dilation(mask);
	Mat temp = mor.currentImg;
	imshow("dialtaion", mor.currentImg);
	cv::imwrite("C:\\DIP\\OUTPUT_IMAGES\\week2\\" + img_name + "_dilation_full_mask.png", mor.currentImg);
	/*
	Mat histImage = Histogram(image).HistImage;
	imshow("histImage", histImage);
	
	cv::imshow("his", image);
	cv::imwrite("C:\\DIP\\OUTPUT_IMAGES\\week2\\" + img_name +"_histogram.png", histImage);
	*/
	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}

