#pragma once
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
using namespace cv;
Mat AffineScaling(double cx, double cy);
Mat AffineTransform(Mat image, Mat affine);
Mat AffineTranslation(int x, int y);
Mat AffineShear(double vertical, double horizontal);
Mat AffineRotate(double seta, int height, int width);
Mat AffineTransform(Mat image, Mat affine, double extend);
Mat AffineTransform(Mat image, Mat affine, double extend, int interpol);