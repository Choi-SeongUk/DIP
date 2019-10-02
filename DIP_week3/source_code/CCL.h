#pragma once
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>
#include <queue>
#include <set>
#include <map>
#include "ColorTransform.h"
#define pi 3.14159265358979323846264338327950288419716939937510582097494459230781
using namespace cv;
using namespace std;
class CCL
{
	
public:
	/*@neighbor_size: 1은 4방향, 2이상은 8방향*/
	void TwoPass(int neighbor_size=1);
	void Pass1(int neighbor_size);
	void PassAtOnce2(int neighbor_size);
	/*@neighbor_size: 1은 4방향, 2이상은 8방향*/
	void PassAtOnce(int neighbor_size = 1); 	
	Mat ToNumberLabeling();
	Mat ToColorImage();
	CCL();
	CCL(Mat image);
	~CCL();
	int label_max = 0;
	Mat LabelImg;
private:
	Mat originImg;
	
	bool is_one_passed = false;
};

