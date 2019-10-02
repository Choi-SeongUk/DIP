#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>
#include <math.h>
#include "SpatialFilter.h"
using namespace cv;
using namespace std;
Mat GetSobelImage(Mat origin)
{
	Mat kernel = GetSobelKernel(0);
	Mat sobel_x = SpatialFilteringWithKernel(origin, kernel);
	kernel = GetSobelKernel(1);
	Mat sobel_y = SpatialFilteringWithKernel(origin, kernel);

	sobel_x = abs(sobel_x);



	sobel_y = abs(sobel_y);


	Mat output;
	cv::add(sobel_x, sobel_y, output);
	return output;

}
double conv2d(Mat m1, Mat m2)
{
	m1.convertTo(m1, CV_64FC1); // 이래도 되나?
	//m2.convertTo(m2, CV_64FC1);
	int sum = 0;
	for (int i = 0; i < m1.rows; i++)
		for (int j = 0; j < m1.cols; j++)
			sum += *(m1.ptr<double>(m1.rows - 1 - i) + m1.cols - 1 - j) * *(m2.ptr<double>(i) + j);
	return sum;
}
Mat GetGaussainKernel(int size, double sigma)
{
	Mat kernel = Mat(size, size, CV_64FC1);

	for (int x = 0; x < size; x++)
		for (int y = 0; y < size; y++)
		{
			double jisu = ((x - size / 2)*(x - size / 2) + (y - size / 2)* (y - size / 2)) / (2 * sigma);
			kernel.at<double>(x, y) = exp(-jisu);
		}
	kernel = kernel / cv::sum(kernel);
	return kernel;
}
Mat GetLaplacianKernel(int direct)
{
	Mat kernel = (Mat_<double>(3, 3) << 1, 1, 1, 1, -8, 1, 1, 1, 1);
	if (direct)
		kernel = -kernel;
	cout << kernel;
	return kernel;
}
Mat GetSobelKernel(int direct)
{
	Mat kernel;
	if (direct == 0)
		kernel = (Mat_<double>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
	else
		kernel = (Mat_<double>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
	cout << kernel;
	return kernel;
}
Mat GetBoxKernel(int sizex, int sizey)
{
	Mat kernel = Mat::ones(Point(sizex, sizey), CV_64FC1);
	kernel = kernel / (sizex*sizey);
	return kernel;
}
Mat GetBoxKernel(int size)
{
	Mat kernel = Mat::ones(Point(size, size), CV_64FC1);
	kernel = kernel / (size*size);
	return kernel;
}
Mat SpatialFilteringWithKernel(Mat origin, Mat kernel)
{
	Mat image_filtered = Mat(origin.rows, origin.cols, CV_64FC1);
	//origin.convertTo(origin, CV_64FC1);
	for (int x = 0; x < origin.rows; x++)
		for (int y = 0; y < origin.cols; y++)
		{
			double sum = 0;
			for (int i = 0; i < kernel.rows; i++)
				for (int j = 0; j < kernel.cols; j++)
				{
					int origin_x = i + x - kernel.rows / 2;
					//origin_x = kernel.rows - 1 - i + x - kernel.rows / 2;
					int origin_y = j + y - kernel.cols / 2;
					//origin_y = kernel.rows - 1 - j + y - kernel.cols / 2;
					if (origin_x < 0)
						origin_x = 0;
					else if (origin_x >= origin.rows)
						origin_x = origin.rows - 1;
					if (origin_y < 0)
						origin_y = 0;
					else if (origin_y >= origin.cols)
						origin_y = origin.cols - 1;
					sum += *(kernel.ptr<double>(i) + j) * *(origin.ptr<uchar>(origin_x) + origin_y);
				}
			*(image_filtered.ptr<double>(x) + y) = sum;
		}
	//*(image_filtered.ptr<double>(x + kernel.rows / 2) + y + kernel.cols / 2) = conv2d(kernel, origin(Rect(y,x, kernel.rows, kernel.cols)));
//	cout << origin(Rect(x, y, kernel.rows, kernel.cols));
//image_filtered.convertTo(image_filtered, CV_8U);
	return image_filtered;
}
Mat SaltAndPepper(Mat origin, double probability)
{
	Mat image_noised = Mat(origin.rows, origin.cols, CV_8U);

	srand((unsigned int)time(NULL));
	for (int x = 0; x < origin.rows; x++)
		for (int y = 0; y < origin.cols; y++)
		{
			int rnd = rand() % 10000;
			if (rnd < probability * 5000)
				image_noised.at<uchar>(x, y) = 0;
			else if (rnd < probability * 10000)
				image_noised.at<uchar>(x, y) = 255;
			else
				image_noised.at<uchar>(x, y) = origin.at<uchar>(x, y);
		}
	return image_noised;
}
Mat MedianFiltering(Mat origin, int size)
{
	Mat image_filtered = Mat(origin.rows, origin.cols, CV_8U);
	for (int x = 0; x < origin.rows; x++)
		for (int y = 0; y < origin.cols; y++)
		{
			Mat sorted;
			int x_start = x - size / 2;
			int y_start = y - size / 2;
			if (x_start < 0)
				x_start = 0;
			else if (x_start >= origin.rows - size)
				x_start = origin.rows - size;
			if (y_start < 0)
				y_start = 0;
			else if (y_start >= origin.cols - size)
				y_start = origin.cols - size;
			cv::sort(origin(Rect(y_start, x_start, size, size)), sorted, SORT_ASCENDING);
			int median = sorted.at<uchar>(size / 2, size / 2);
			image_filtered.at<uchar>(x, y) = median;
		}
	return image_filtered;
}
Mat AdaptiveMedianFiltering(Mat origin, int maxsize)
{
	int size = 3;
	Mat image_filtered = Mat(origin.rows, origin.cols, CV_8U);
	for (int x = 0; x < origin.rows; x++)
		for (int y = 0; y < origin.cols; y++)
		{
			Mat sorted;
			int x_start = x - size / 2;
			int y_start = y - size / 2;
			if (x_start < 0)
				x_start = 0;
			else if (x_start >= origin.rows - size)
				x_start = origin.rows - size;
			if (y_start < 0)
				y_start = 0;
			else if (y_start >= origin.cols - size)
				y_start = origin.cols - size;
			cv::sort(origin(Rect(y_start, x_start, size, size)), sorted, SORT_ASCENDING);
			int median = *(sorted.ptr<uchar>(size / 2) + size / 2);
			int min = *sorted.ptr<uchar>(0);
			int max = *(sorted.ptr<uchar>(size - 1) + size - 1);
			int zxy = *(origin.ptr<uchar>(x) + y);
			if (median > min && median < max)
			{
				if (zxy > min && zxy < max)
					*(image_filtered.ptr<uchar>(x) + y) = zxy;
				else
					*(image_filtered.ptr<uchar>(x) + y) = median;
				continue;
			}
			else
			{
				size++;
				if (size > maxsize)
				{
					*(image_filtered.ptr<uchar>(x) + y) = median;
					size = 3;
				}
				else
					y--;
			}
		}
	return image_filtered;
}
Mat MatAdd(Mat mat1, Mat mat2, int threshold)
{
	Mat image_diff = Mat(mat1.rows, mat1.cols, CV_32SC1);
	for (int x = 0; x < mat1.rows; x++)
		for (int y = 0; y < mat1.cols; y++)
		{
			if (mat2.at<uchar>(x, y) > threshold)
				image_diff.at<int>(x, y) = (mat2.at<uchar>(x, y) + mat1.at<uchar>(x, y));
			else
				image_diff.at<int>(x, y) = (mat1.at<uchar>(x, y));
		}
	image_diff.convertTo(image_diff, CV_8U);
	return (image_diff);
}


Mat IntesityMinMax(Mat origin, int min, int max)
{
	Mat image_sacled;
	origin.copyTo(image_sacled);
	image_sacled = image_sacled - min;
	image_sacled = image_sacled * 255 / (max - min);
	image_sacled.convertTo(image_sacled, CV_8U);
	return image_sacled;
}
Mat IntensityScale(Mat origin)
{
	double min = 0;
	double max = 0;
	for (int x = 0; x < origin.rows; x++)
		for (int y = 0; y < origin.cols; y++)
		{
			if (*(origin.ptr<double>(x) + y) < min)
				min = *(origin.ptr<double>(x) + y);
			if (*(origin.ptr<double>(x) + y) > max)
				max = *(origin.ptr<double>(x) + y);
		}
	cout << "min: " << min;
	cout << " max: " << max;

	return IntesityMinMax(origin, min, max);
}

Mat GetUnsharpMask(Mat origin, int size, double sigma)
{
	Mat mask;
	Mat kernel = GetGaussainKernel(size, sigma);
	Mat filtered = SpatialFilteringWithKernel(origin, kernel);
	Mat origin64;
	origin.convertTo(origin64, CV_64F);
	add(origin64, -filtered, mask);
	return mask;
}