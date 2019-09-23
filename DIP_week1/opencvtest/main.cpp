#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>
#include <cstdlib>
#include <math.h>
#include <time.h>
#include "Affine.h"
#include "SpatialFilter.h"
#include "IntensityTransform.h"
#include "ColorTransform.h"
#define pi 3.14159265358979323846264338327950288419716939937510582097494459230781

using namespace cv;
using namespace std;

//histogram을 그리기 위함 
void histogram(int histSize, const float*ranges[], int channels, int dims)
{
	Mat srcImage = imread("lena.jpg", IMREAD_GRAYSCALE);
	if (srcImage.empty())
		return;
	Mat hist;
	calcHist(&srcImage, 1, &channels, Mat(), hist, 1, &histSize, ranges);


	Mat histImage(512, 512, CV_8U);
	normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, CV_32F);

	histImage = Scalar(255);
	int binW = cvRound((double)histImage.cols / histSize);
	int x1, y1, x2, y2;
	for (int i = 0; i < histSize; i++)
	{
		x1 = i * binW;
		y1 = histImage.rows;
		x2 = (i + 1)*binW;
		y2 = histImage.rows - cvRound(hist.at<float>(i));
		rectangle(histImage, Point(x1, y1), Point(x2, y2), Scalar(0), -1);
	}
	imshow("histImage", histImage);
}

int main()
{
	cv::Mat image = cv::imread("C:\\DIP\\IMAGES\\lena_256.png", cv::IMREAD_GRAYSCALE);
	int histogram[255] = { 0 };

	for(int x=0;x<image.rows;x++)
		for (int y = 0; y < image.cols; y++)
		{
			int intensity = image.at<uchar>(x, y);
			histogram[intensity]++;
		}

	Mat histImage = Mat(512, 512, CV_8U);

	histImage = Scalar(255);
	int binW = cvRound((double)histImage.cols / 256);
	imshow("histImage2", histImage);
	int x1, y1, x2, y2;
	for (int i = 0; i < 255; i++)
	{
		x1 = i * binW ;
		y1 = histImage.rows- (histogram[i])/10;
		x2 = (i + 1)*binW ;
		y2 = histImage.rows;
		rectangle(histImage, Point(x1, y1), Point(x2, y2), 5,-1);
	}
	imshow("histImage", histImage);

	
	cv::imshow("his", image);
	//	cv::imwrite("C:\\DIP\\OUTPUT_IMAGES\\lena_256_2배_bilinear.png", image2_r);

	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}

