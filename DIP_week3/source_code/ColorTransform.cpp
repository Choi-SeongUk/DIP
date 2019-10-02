#include "ColorTransform.h"
#include "IntensityTransform.h"
#include <iostream>
#include <math.h>

RGB_MAT::RGB_MAT(Mat colorImage)
{
	Mat bgr[3];
	Mat img_64;
	colorImage.convertTo(img_64, CV_64F);
	split(img_64, bgr);
	this->R = bgr[2];
	this->G = bgr[1];
	this->B = bgr[0];
}

RGB_MAT::RGB_MAT(Mat R, Mat G, Mat B)
{
	R.convertTo(this->R, CV_64F);
	G.convertTo(this->G, CV_64F);
	B.convertTo(this->B, CV_64F);
}

Mat RGB_MAT::ToImage()
{
	Mat bgr[3] = { this->B, this->G,this->R };
	Mat img_color;
	cv::merge(bgr, 3, img_color);
	img_color.convertTo(img_color, CV_8U);
	return img_color;
}

HSI_MAT RGB_MAT::ToHSI()
{
	
	Mat hsi[3];
	hsi[0] = Mat(this->R.rows, this->R.cols, CV_64F);
	hsi[1] = Mat(this->R.rows, this->R.cols, CV_64F);
	hsi[2] = Mat(this->R.rows, this->R.cols, CV_64F);
	for (int x = 0; x < this->R.rows; x++)
		for (int y = 0; y < this->R.cols; y++)
		{
			double r = *(this->R.ptr<double>(x) + y);
			double g = *(this->G.ptr<double>(x) + y);
			double b = *(this->B.ptr<double>(x) + y);
			if (r > 255)
				r = 255;
			else if (r < 0)
				r = 0;
			if (g > 255)
				g = 255;
			else if (g < 0)
				g = 0;
			if (b > 255)
				b = 255;
			else if (b < 0)
				b = 0;

			double i = (r + g + b) / 3;
			double s = 1 - min(min(r, g), b) * 3 / (r + g + b);
			double h = 0;
			if (r != g || g != b)
			{
				double iseta_denomi = (r - g)*(r - g) + (r - b)*(g - b);
				iseta_denomi = sqrt(iseta_denomi);
				double aseta = 0.5*((r - g) + (r - b)) / iseta_denomi;
				double seta = acos(aseta);
				if (b > g)
					h = 2 * pi - seta;
				else
					h = seta;
			}
			*(hsi[0].ptr<double>(x) + y) = h;
			*(hsi[1].ptr<double>(x) + y) = s;
			*(hsi[2].ptr<double>(x) + y) = i;
		}

	HSI_MAT output = HSI_MAT(hsi[0],hsi[1],hsi[2]);

	return output;
}


HSI_MAT::HSI_MAT(Mat H, Mat S, Mat I)
{
	H.convertTo(this->H, CV_64F);
	S.convertTo(this->S, CV_64F);
	I.convertTo(this->I, CV_64F);
}

RGB_MAT HSI_MAT::ToRGB()
{
	Mat rgb[3];
	rgb[0] = Mat(this->H.rows, this->H.cols, CV_64F);
	rgb[1] = Mat(this->H.rows, this->H.cols, CV_64F);
	rgb[2] = Mat(this->H.rows, this->H.cols, CV_64F);
	for (int x = 0; x < this->H.rows; x++)
		for (int y = 0; y < this->H.cols; y++)
		{
			double h = *(this->H.ptr<double>(x) + y);
			double s = *(this->S.ptr<double>(x) + y);
			double i = *(this->I.ptr<double>(x) + y);
			double r, g, b;
			s = s > 1 ? 1 : s;
			s = s < 0 ? 0 : s;
			while (h < 0)
				h = 2 * pi + h;
			while (h > 2 * pi)
				h = h - 2*pi;
			if (i > 255)
				i = 255;
			else if (i < 0)
				i = 0;
			if (h < 2 * pi / 3)
			{
				b = i*(1 - s);
				r = i * (1 + s * cos(h) / cos(pi / 3 - h));
				g = 3 * i - (r + b);
			}
			else if (h < 4 * pi / 3)
			{
				h = h - 2 * pi / 3;
				r = i * (1 - s);
				g = i * (1 + s * cos(h) / cos(pi / 3 - h));
				b = 3 * i - (r + g);
			}
			else
			{
				h = h - 4 * pi / 3;
				g = i * (1 - s);
				b = i * (1 + s * cos(h) / cos(pi / 3 - h));
				r = 3 * i - (g + b);
			}
			*(rgb[0].ptr<double>(x) + y) = round(r);
			*(rgb[1].ptr<double>(x) + y) = round(g);
			*(rgb[2].ptr<double>(x) + y) = round(b);
		}

	return RGB_MAT(rgb[0],rgb[1],rgb[2]);
}
