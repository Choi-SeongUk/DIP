#include "Threshold.h"
#include "Histogram.h"
#include <vector>

Threshold::Threshold()
{
}

Threshold::Threshold(Mat image)
{
	this->currentImg = image;
}
void Threshold::BasicTh(int i_th)
{
	for (int x = 0; x < this->currentImg.rows; x++)
		for (int y = 0; y < this->currentImg.cols; y++)
			if (*(currentImg.ptr(x) + y) <= i_th)
				*(currentImg.ptr(x) + y) = 0;
			else
				*(currentImg.ptr(x) + y) = 255;
}
void Threshold::TripleTh(int k1,int k2)
{
	for (int x = 0; x < this->currentImg.rows; x++)
		for (int y = 0; y < this->currentImg.cols; y++)
			if (*(currentImg.ptr(x) + y) <= k1)
				*(currentImg.ptr(x) + y) = 0;
			else if(*(currentImg.ptr(x) + y) <= k2)
				*(currentImg.ptr(x) + y) = 127;
			else
				*(currentImg.ptr(x) + y) = 255;
}
void Threshold::Partial_Th()
{
	Threshold subTh[2][3];
	
	int subRows = currentImg.rows / 2;
	int subCols = currentImg.cols / 3;
	Mat newImg = Mat(subRows*2, subCols*3, CV_8U);
	for(int row=0;row<2;row++)
		for (int col = 0; col < 3; col++)
		{
			Mat subimage = Mat(subRows, subCols, CV_8U);
			for (int x = 0; x < subRows; x++)
				for (int y = 0; y < subCols; y++)
					*(subimage.ptr(x) + y) = *(this->currentImg.ptr(x + row * subRows) + (y + col * subCols));
			subTh[row][col] = Threshold(subimage);
			subTh[row][col].Otsu();
			for (int x = 0; x < subRows; x++)
				for (int y = 0; y < subCols; y++)
					*(this->currentImg.ptr(x + row * subRows) + (y + col * subCols)) = *(subimage.ptr(x) + y);
		}
}
void Threshold::Otsu_tripe()
{
	Histogram his = Histogram(this->currentImg);
	std::array<array<double,256>,256> sigma;
	std::vector<Point> maxList;
	double m_g = his.GetCumulativeMean(0, 255);
	double max = 0;
	for (int k1 = 1; k1 <= 255; k1++)
		for (int k2 = k1; k2 <= 255; k2++)
		{
			double p1 = his.GetCumulativeSum(0, k1);
			double p2 = his.GetCumulativeSum(k1 + 1, k2);
			double p3 = 1 - p1 - p2;
			double m1 = his.GetMeanIntensity(0, k1);
			double m2 = his.GetMeanIntensity(k1 + 1, k2);
			double m3 = his.GetMeanIntensity(k2 + 1, 255);
			double temp = p1 * (m1 - m_g)*(m1 - m_g);
			temp += p2 * (m2 - m_g)*(m2 - m_g);
			temp += p3 * (m3 - m_g)*(m3 - m_g);
			sigma[k1][k2] = temp;
			if (temp >= max)
				max = temp;
		}

	for (int k1 = 1; k1 <= 255; k1++)
		for (int k2 = k1; k2 <= 255; k2++)
		{
			Point a;
			a.x = k1;
			a.y = k2;
			if (sigma[k1][k2] == max)
				maxList.push_back(a);
		}

	double k1_otsu = 0;
	double k2_otsu = 0;
	for (int i = 0; i < maxList.size(); i++)
	{
		k1_otsu += maxList[i].x;
		k2_otsu += maxList[i].y;
	}
	k1_otsu /= maxList.size();
	k2_otsu /= maxList.size();
	TripleTh(k1_otsu, k2_otsu);
}
void Threshold::Otsu()
{
	Histogram his = Histogram(this->currentImg);
	std::array<double, 256> sigma;
	std::vector<int> maxList;
	double m_g = his.GetCumulativeMean(0,255);
	double max = 0;
	for (int k = 0; k < 256; k++)
	{
		double p1 = his.GetCumulativeSum(0, k);
		double m = his.GetCumulativeMean(0, k);
		double temp  = (m_g * p1 - m)*(m_g * p1 - m);
		temp = temp / (p1 * (1 - p1));
		if (temp >= max)
			max = temp;
		sigma[k] = temp;
	}
	for (int k = 0; k < 256; k++)
	{
		if (sigma[k] == max)
			maxList.push_back(k);
	}
	double k_otsu =0;
	for (int i = 0; i < maxList.size(); i++)
		k_otsu += maxList[i];
	k_otsu /= maxList.size();
	BasicTh(k_otsu);
}
void Threshold::GlobalTh(int intialtT, double deltaT)
{
	
	Histogram his = Histogram(this->currentImg);
	std::array<int, 256> histogram = his.GetHistogram();

	double m1 =0 ,m2=0;
	double sum = 0;
	for (int i = 0; i <= intialtT; i++)
	{
		sum += histogram[i];
		m1 += histogram[i] * i;
	}
	m1 = m1 / sum;
	sum = 0;
	for (int i = intialtT+1; i <= 255; i++)
	{
		sum += histogram[i];
		m2 += histogram[i] * i;
	}
	m2 = m2 / sum;
	int newT = round((m1 + m2) / 2);
	if (abs(newT - intialtT) <= deltaT)
		BasicTh(newT);
	else
		GlobalTh(newT, deltaT);
}

void Threshold::LocalTh(int neigborSize, int a, int b)
{
	double mean_g = Histogram(this->currentImg).GetMeanIntensity(0, 255);
	Mat newImg = Mat(this->currentImg.rows, this->currentImg.cols, CV_8U);
	for (int x = 0; x < this->currentImg.rows; x++)
		for (int y = 0; y < this->currentImg.cols; y++)
		{
			double sigma = 0;
			double mean=0;
			for(int i=x-neigborSize/2;i<=x+neigborSize/2;i++)
				for (int j = y - neigborSize / 2; j <= y + neigborSize / 2; j++)
				{
					int pad_i = i;
					int pad_j = j;
					if (pad_i < 0)
						pad_i = 0;
					else if (pad_i >= currentImg.rows)
						pad_i = currentImg.rows - 1;
					if (pad_j < 0)
						pad_j = 0;
					else if (pad_j >= currentImg.cols)
						pad_j = currentImg.cols - 1;
					mean += *(currentImg.ptr(pad_i) + pad_j);
				}
			mean /= (neigborSize*neigborSize);
			for (int i = x - neigborSize / 2; i <= x + neigborSize / 2; i++)
				for (int j = y - neigborSize / 2; j <= y + neigborSize / 2; j++)
				{
					int pad_i = i;
					int pad_j = j;
					if (pad_i < 0)
						pad_i = 0;
					else if (pad_i >= currentImg.rows)
						pad_i = currentImg.rows - 1;
					if (pad_j < 0)
						pad_j = 0;
					else if (pad_j >= currentImg.cols)
						pad_j = currentImg.cols - 1;

					sigma += pow(*(currentImg.ptr(pad_i) + pad_j) - mean,2);
				}
			sigma /= (neigborSize*neigborSize);
			sigma = sqrt(sigma);

			int fxy = *(currentImg.ptr(x) + y);
			if (fxy > a * sigma && fxy > b * mean_g)
				*(newImg.ptr(x) + y) = 255;
			else
				*(newImg.ptr(x) + y) = 0;
			*(newImg.ptr(x) + y) = b* mean_g;
		}
	this->currentImg = newImg;
}
Threshold::~Threshold()
{
}
