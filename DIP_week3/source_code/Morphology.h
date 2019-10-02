#pragma once
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
using namespace cv;
class Morphology
{
public:
	void test();
	void Erosion(Mat mask);
	bool Dilation2(Mat se);
	void Dilation(Mat mask);
	void Opening(Mat mask);
	Morphology();
	Morphology(Mat in);
	void BoundaryExtract(Mat se);


	void HoleFill(Mat mask, Point location);
	bool GetNextConnect(Mat mask, Mat refImg);
	//bool GeoDilation(Mat se, Mat refImg);
	bool GeoDilation2(Mat se, Mat refImg, Mat preImg);
	void OpeningByR(Mat se_erosion, Mat se_reconstruct,int n_erosion);
	void ReconstructByD(Mat se, Mat refImg);
	void HoleFillByR(Mat se);
	void BorderCleanByR(Mat se);
	void DetectConnect(Mat mask, Point location);
	Mat currentImg;
private:
	int high = 250;
	int low = 10;
};

