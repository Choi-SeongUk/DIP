#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
using namespace cv;
double conv2d(Mat m1, Mat m2);
Mat GetGaussainKernel(int size, double sigma);
Mat GetLaplacianKernel(int direct);
Mat GetSobelKernel(int direct);
Mat GetBoxKernel(int sizex, int sizey);
Mat GetBoxKernel(int size);
Mat SpatialFilteringWithKernel(Mat origin, Mat kernel);
Mat SaltAndPepper(Mat origin, double probability);
Mat MedianFiltering(Mat origin, int size);
Mat AdaptiveMedianFiltering(Mat origin, int maxsize);
Mat MatAdd(Mat mat1, Mat mat2, int threshold);
Mat GetSobelImage(Mat origin);
Mat IntesityMinMax(Mat origin, int min, int max);
Mat IntensityScale(Mat origin);
Mat GetUnsharpMask(Mat origin, int size, double sigma);