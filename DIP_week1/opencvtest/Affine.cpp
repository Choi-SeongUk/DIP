#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>
#include <math.h>

#define pi 3.14159265358979323846264338327950288419716939937510582097494459230781


using namespace cv;
using namespace std;

Mat AffineTransform(Mat image, Mat affine)
{

	cv::Mat image_moved = Mat(image.rows, image.cols, CV_32SC1);

	Mat source = (Mat_<double>(1, 3) << 0, 0, 1);
	Mat inv_affine = affine.inv();
	for (int x = 0; x < image.rows; x++)
	{
		for (int y = 0; y < image.cols; y++)
		{
			*source.ptr<double>(0) = x;
			*(source.ptr<double>(0) + 1) = y;


			/*	Mat des = source * inv_affine;
				int new_x = round(*des.ptr<double>(0));
				int new_y = round(*(des.ptr<double>(0) +1));
				*/
			double new_x = 0, new_y = 0;
			int i = 0;
			for (int j = 0; j < 3; j++)
				new_x += *(source.ptr<double>(0) + j) * *(inv_affine.ptr<double>(j) + i);
			i = 1;
			for (int j = 0; j < 3; j++)
				new_y += *(source.ptr<double>(0) + j) * *(inv_affine.ptr<double>(j) + i);

			new_x = round(new_x);
			new_y = round(new_y);

			if (new_x < 0 || new_y < 0)
				continue;
			else if (new_x >= image.rows || new_y >= image.cols)
				continue;
			else
				*(image_moved.ptr<int>(x) + y) = *(image.ptr<uchar>(new_x) + int(new_y));
		}
	}

	image_moved.convertTo(image_moved, CV_8U);
	return (image_moved);
}

Mat AffineTransform(Mat image, Mat affine, double extend)
{
	int new_height = image.size().height*extend;
	int new_width = image.size().width*extend;
	int x_trans = (new_height - image.rows) / 2.0;
	int y_trans = (new_width - image.cols) / 2.0;

	Mat image_moved = Mat(new_height, new_width, CV_32SC1);
	Mat source = (Mat_<double>(1, 3) << 0, 0, 1);
	Mat inv_affine = affine.inv();

	for (int x = -x_trans; x < new_height - x_trans; x++)
	{
		for (int y = -y_trans; y < new_width - y_trans; y++)
		{
			*source.ptr<double>(0) = x;
			*(source.ptr<double>(0) + 1) = y;
			double new_x = 0;
			double new_y = 0;
			int i = 0;
			for (int j = 0; j < 3; j++)
				new_x += *(source.ptr<double>(0) + j) * *(inv_affine.ptr<double>(j) + i);
			i = 1;
			for (int j = 0; j < 3; j++)
				new_y += *(source.ptr<double>(0) + j) * *(inv_affine.ptr<double>(j) + i);

			new_x = round(new_x);
			new_y = round(new_y);
			
			if (new_x < 0 || new_y < 0)
				continue;
			else if (new_x >= image.rows || new_y >= image.cols)
				continue;
			else
			{
				*(image_moved.ptr<int>(x + x_trans) + y + y_trans) = *(image.ptr<uchar>(new_x) + int(new_y));
			}
		}
	}

	image_moved.convertTo(image_moved, CV_8U);
	return (image_moved);
}


Mat AffineTransform(Mat image, Mat affine, double extend, int interpol)
{
	int new_height = image.size().height*extend;
	int new_width = image.size().width*extend;
	Mat image_moved = Mat(new_height, new_width, CV_32SC1);
	Mat source = (Mat_<double>(1, 3) << 0, 0, 1);
	Mat inv_affine = affine.inv();

	for (int x = 0; x < new_height ; x++)
	{
		for (int y = 0; y < new_width ; y++)
		{
			*source.ptr<double>(0) = x;
			*(source.ptr<double>(0) + 1) = y;
			double new_x = 0;
			double new_y = 0;
			int i = 0;
			for (int j = 0; j < 3; j++)
				new_x += *(source.ptr<double>(0) + j) * *(inv_affine.ptr<double>(j) + i);
			i = 1;
			for (int j = 0; j < 3; j++)
				new_y += *(source.ptr<double>(0) + j) * *(inv_affine.ptr<double>(j) + i);

			if (new_x < 0 || new_y < 0)
				continue;
			else if (round(new_x) >= image.rows || round(new_y) >= image.cols)
				continue;
			else
			{

				if (interpol != 0)
				{
					int x1, x2, y1, y2;
					int intensity_y1, intensity_y2;
					x1 = (int)new_x;
					x2 = x1 + 1;
					y1 = (int)new_y;
					y2 = y1 + 1;
					double x2weight = (x2 - new_x);
					double y2weight = (y2 - new_y);
					if (x2 >= image.rows)
						x2 = x2 - 1;
					if (y2 >= image.cols)
						y2 = y2 - 1;

					intensity_y1 = (new_x - x1) * *(image.ptr<uchar>(x2) + y1) + x2weight * *(image.ptr<uchar>(x1) + (y1));
					intensity_y2 = (new_x - x1) * *(image.ptr<uchar>(x2) + (y2)) + x2weight * *(image.ptr<uchar>(x1) + (y2));

					*(image_moved.ptr<int>(x) + y) = y2weight *intensity_y1 + (new_y - y1)*intensity_y2;
				}
				else
				{
					new_x = round(new_x);
					new_y = round(new_y);
					*(image_moved.ptr<int>(x) + y) = *(image.ptr<uchar>(new_x) + int(new_y));

				}
			}
		}
	}

	image_moved.convertTo(image_moved, CV_8U);
	return (image_moved);
}

Mat AffineTranslation(int x, int y)
{
	Mat affine = (Mat_<double>(3, 3) << 1, 0, 0, 0, 1, 0, x, y, 1);
	return affine;
}

Mat AffineScaling(double cx, double cy)
{
	Mat affine = (Mat_<double>(3, 3) << cx, 0, 0, 0, cy, 0, 0, 0, 1);
	cout << affine;
	return affine;
}

Mat AffineShear(double vertical, double horizontal)
{
	Mat affine = (Mat_<double>(3, 3) << 1, horizontal, 0, vertical, 1, 0, 0, 0, 1);
	return affine;
}

Mat AffineRotate(double seta, int height, int width)
{
	int m = height / 2;
	int n = width / 2;
	Mat affine_trans = AffineTranslation(-m, -n);
	double radi = seta * pi / 180;
	Mat affine = (Mat_<double>(3, 3) << cos(radi), sin(radi), 0, -sin(radi), cos(radi), 0, m*(1 - cos(radi)) + n * sin(radi), n*(1 - cos(radi)) - m * sin(radi), 1);
	Mat affine_inv_tras = AffineTranslation(m, n);
	Mat affine_result = affine_trans * affine * affine_inv_tras;

	return affine;
}