#include "Morphology.h"
#include <time.h>
#include <iostream>

void Morphology::test()
{
	clock_t start, end;
	start = clock();
	int ii = 0;

	for (ii = 0; ii < 10; ii++)
	{
		for (int i = 0; i < this->currentImg.rows; i++)
			for (int j = 0; j < this->currentImg.cols; j++)
			{
				int image_pixel = i * j;

				if (image_pixel > 10)
					image_pixel = 10;
				int d = *(this->currentImg.ptr<uchar>(i) + j);
				image_pixel = d;
				image_pixel = d + 2;
			}
	}
	end = clock();
	std::cout << (double)(end - start);
}
void Morphology::Erosion(Mat se)
{
	Mat newImg = Mat(this->currentImg.rows,this->currentImg.cols,CV_8U);
	newImg = Scalar(0);
	for (int x = 0; x < this->currentImg.rows; x++)
		for (int y = 0; y < this->currentImg.cols; y++)
		{
			bool erose = false;
			for (int i = -se.rows / 2; i < se.rows / 2 + 1; i++)
				for (int j = -se.cols / 2; j < se.cols / 2 + 1; j++)
				{				
					int image_pixel;
					if (y + j < 0 || y + j >= this->currentImg.cols)
						image_pixel = high;
					else if (x + i < 0 || x + i >= this->currentImg.rows)
						image_pixel = high;
					else
						image_pixel = *(this->currentImg.ptr<uchar>(x + i) + y + j);
					int mask_pixel = *(se.ptr<uchar>(i + se.rows / 2) + j + se.cols / 2);
					if (image_pixel <= low && mask_pixel >= high )
					{
						erose = true;
						goto unlockloop;
					}
				}
unlockloop:
			if(!erose)
				*(newImg.ptr<uchar>(x)+y) = *(this->currentImg.ptr<uchar>(x)+ y);
		}
	this->currentImg = newImg;
}
bool Morphology::Dilation2(Mat se)
{
	bool change = false;
	Mat newImg = Mat(this->currentImg.rows, this->currentImg.cols, CV_8U);
	newImg = Scalar(0);
	for (int y = 0; y < this->currentImg.cols; y++)
		for (int x = 0; x < this->currentImg.rows; x++)
		{
			bool dilation = false;
			for (int j = -se.cols / 2; j < se.cols / 2 + 1; j++)
				for (int i = -se.rows / 2; i < se.rows / 2 + 1; i++)
				{
					int image_pixel;
					if (y + j < 0 || y + j >= this->currentImg.cols)
						image_pixel = low;
					else if (x + i < 0 || x + i >= this->currentImg.rows)
						image_pixel = low;
					else
						image_pixel = *(this->currentImg.ptr<uchar>(x + i) + y + j);
					int mask_pixel = *(se.ptr<uchar>(-i + se.rows / 2) - j + se.cols / 2);
					if (image_pixel >= high && mask_pixel >= high)
					{
						dilation = true;
						goto unlockloop;
					}
				}
		unlockloop:
			if (dilation)
			{
				*(newImg.ptr<uchar>(x) + y) = 255;
				if (*(this->currentImg.ptr<uchar>(x) + y) <= low)
					change = true;
			}
		}
	this->currentImg = newImg;
	return change;
}
void Morphology::Dilation(Mat se)
{
	Mat newImg = Mat(this->currentImg.rows, this->currentImg.cols, CV_8U);
	newImg = Scalar(0);
	for (int x = 0; x < this->currentImg.rows; x++)
		for (int y = 0; y < this->currentImg.cols; y++)
		{
			bool dilation = false;
			for (int i = -se.rows / 2; i < se.rows / 2 + 1; i++)
				for (int j = -se.cols / 2; j < se.cols / 2 + 1; j++)
				{
					int image_pixel;
					if (y + j < 0 || y + j >= this->currentImg.cols)
						image_pixel = low;
					else if (x + i < 0 || x + i >= this->currentImg.rows)
						image_pixel = low;
					else
						image_pixel = *(this->currentImg.ptr<uchar>(x + i) + y + j);
					int mask_pixel = *(se.ptr<uchar>(-i + se.rows / 2) - j + se.cols / 2);
					if (image_pixel >=high && mask_pixel >=high )
					{
						dilation = true;
						goto unlockloop;
					}
				}
		unlockloop:
			if (dilation)
				*(newImg.ptr<uchar>(x) + y) = 255;
		}
	this->currentImg = newImg;
}
void Morphology::Opening(Mat se)
{
	this->Erosion(se);
	this->Dilation2(se);
}
Morphology::Morphology()
{
}
Morphology::Morphology(Mat in)
{
	in.copyTo(currentImg);
}


void Morphology::BoundaryExtract(Mat se)
{
	Mat temp;
	this->currentImg.convertTo(temp, CV_32F);
	Erosion(se);
	Mat current32Img;
	this->currentImg.convertTo(current32Img, CV_32F);
	temp = temp - current32Img;
	temp.convertTo(this->currentImg, CV_8U);
}


void Morphology::HoleFill(Mat se, Point location)
{
	Mat x0 = Mat(this->currentImg.rows, this->currentImg.cols, CV_8U);
	x0 = 0;
	*(x0.ptr(location.x) + location.y) = 255;
	*(x0.ptr(location.x) + location.y) = 255;
	Morphology m1 = Morphology(x0);
	m1.ReconstructByD(se, 255-this->currentImg);
	this->currentImg = this->currentImg +m1.currentImg ;
		/*while (1)
	{
		bool change = m1.GeoDilation2(se, 255-this->currentImg);
		if (!change)
		{
			x0 = m1.currentImg;
			break;
		}
	}
	this->currentImg = m1.currentImg + this->currentImg;
	*/
}

bool Morphology::GetNextConnect(Mat se, Mat refImg)
{
	bool change = false;
	Mat newImg = Mat(this->currentImg.rows, this->currentImg.cols, CV_8U);
	newImg = Scalar(0);
	for (int y = 0; y < this->currentImg.cols; y++)
		for (int x = 0; x < this->currentImg.rows; x++)
		{
			bool dilation = false;
			if (*(refImg.ptr<uchar>(x) + y) <= low)
				continue;
			for (int j = -se.cols / 2; j < se.cols / 2 + 1; j++)
				for (int i = -se.rows / 2; i < se.rows / 2 + 1; i++)
				{
					int image_pixel;
					if (y + j < 0 || y + j >= this->currentImg.cols)
						image_pixel = low;
					else if (x + i < 0 || x + i >= this->currentImg.rows)
						image_pixel = low;
					else
						image_pixel = *(this->currentImg.ptr<uchar>(x + i) + y + j);
					int mask_pixel = *(se.ptr<uchar>(-i + se.rows / 2) - j + se.cols / 2);
					if (image_pixel >= high && mask_pixel >= high)
					{
						dilation = true;
						goto unlockloop;
					}
				}
		unlockloop:
			if (dilation)
			{
				*(newImg.ptr<uchar>(x) + y) = 255;
				if (*(this->currentImg.ptr<uchar>(x) + y) <= low)
					change = true;
			}
		}
	this->currentImg = newImg;
	return change;
}
/* old geodialation
bool Morphology::GeoDilation(Mat se, Mat refImg)
{
	bool change = false;
	Mat newImg = Mat(this->currentImg.rows, this->currentImg.cols, CV_8U);
	newImg = Scalar(0);
	int se_half_cols = se.cols / 2;
	int se_half_rows = se.rows / 2;
	for (int y = 0; y < this->currentImg.cols; y++)
		for (int x = 0; x < this->currentImg.rows; x++)
		{
			if (*(this->currentImg.ptr<uchar>(x) + y) && *(refImg.ptr<uchar>(x) + y))
			{
				for (int j = -se_half_cols; j <= se_half_cols; j++)
					for (int i = -se_half_rows; i <= se_half_rows; i++)
					{
						int cur_row = x + i;
						int cur_col = y + j;
						if (cur_col >= 0 && cur_col < this->currentImg.cols && cur_row >= 0 && cur_row < this->currentImg.rows)
						{				
							if (*(se.ptr<uchar>(se_half_rows + i) + se_half_cols + j))
							{
								*(newImg.ptr<uchar>(cur_row) + cur_col) = 255;
								if (!*(this->currentImg.ptr<uchar>(cur_row) + cur_col))
									change = true;
							}
						}
					}
			}
		}
		
	this->currentImg = newImg;
	return change;
}
*/
bool Morphology::GeoDilation2(Mat se, Mat refImg,Mat preImg)
{
	bool change = false;
	Mat newImg = Mat(this->currentImg.rows, this->currentImg.cols, CV_8U);
	newImg = Scalar(0);
	int se_half_cols = se.cols / 2;
	int se_half_rows = se.rows / 2;
	for (int y = 0; y < this->currentImg.cols; y++)
		for (int x = 0; x < this->currentImg.rows; x++)
		{
			if (*(this->currentImg.ptr<uchar>(x) + y) && *(refImg.ptr<uchar>(x) + y) && !*(preImg.ptr<uchar>(x) + y))
			{
				for (int j = -se_half_cols; j <= se_half_cols; j++)
					for (int i = -se_half_rows; i <= se_half_rows; i++)
					{
						int cur_row = x + i;
						int cur_col = y + j;
						if (cur_col >= 0 && cur_col < this->currentImg.cols && cur_row >= 0 && cur_row < this->currentImg.rows)
						{
							if (*(se.ptr<uchar>(se_half_rows + i) + se_half_cols + j))
							{
								*(newImg.ptr<uchar>(cur_row) + cur_col) = 255;
								if (!*(this->currentImg.ptr<uchar>(cur_row) + cur_col))
									change = true;
							}
						}
					}
			}
		}

	this->currentImg = newImg + currentImg;
	return change;
}
void Morphology::OpeningByR(Mat se_erosion, Mat se_reconstruction, int n_erosion)
{
	Mat ref_mask;
	this->currentImg.copyTo(ref_mask);
	for(int i=0;i<n_erosion;i++)
		Erosion(se_erosion);
	ReconstructByD(se_reconstruction, ref_mask);
}
void Morphology::ReconstructByD(Mat se, Mat refImg)
{
	Mat preImg1 = Mat(currentImg.rows, currentImg.cols, CV_8U);
	preImg1 = 0;
	Mat preImg2 = Mat(currentImg.rows, currentImg.cols, CV_8U);
	preImg2 = 0;
	int i;
	for( i=1;i;i++)
	{
		if (i > 5)
		{
			int a;
			a = 3;
		}
		bool change = GeoDilation2(se,refImg,preImg1);
		if (!change)
			break;
		preImg1 = preImg2+0;
		preImg2 = currentImg+0;
	}

}
void Morphology::HoleFillByR(Mat se)
{
	Mat ref_mask;
	this->currentImg.copyTo(ref_mask);
	int y = 0;
	currentImg = 0;
	for(int y=0;y<currentImg.cols;y = y +currentImg.cols-1)
		for(int x=0;x<currentImg.rows;x++)
			*(currentImg.ptr(x) + y) = 255 - *(ref_mask.ptr(x)+y);
	for (int x = 0; x < currentImg.rows; x = x + currentImg.rows - 1)
		for (int  y = 0; y < currentImg.cols; y++)
			*(currentImg.ptr(x) + y) = 255 - *(ref_mask.ptr(x) + y);

	ReconstructByD(se, 255 - ref_mask);
	cv::imwrite("C:\\DIP\\week2\\output\\480번수행.png", currentImg);
	this->currentImg = 255 - this->currentImg;
	cv::imwrite("C:\\DIP\\week2\\output\\480번수행 뒤집.png", currentImg);
	this->currentImg = this->currentImg + ref_mask;
	cv::imwrite("C:\\DIP\\week2\\output\\480번수행 플러스 원본.png", currentImg);
}
void Morphology::BorderCleanByR(Mat se)
{
	Mat ref_mask;
	this->currentImg.copyTo(ref_mask);
	currentImg = 0;
	for (int y = 0; y < currentImg.cols; y = y + currentImg.cols - 1)
		for (int x = 0; x < currentImg.rows; x++)
			*(currentImg.ptr(x) + y) = *(ref_mask.ptr(x) + y);
	for (int x = 0; x < currentImg.rows; x = x + currentImg.rows - 1)
		for (int y = 0; y < currentImg.cols; y++)
			*(currentImg.ptr(x) + y) = *(ref_mask.ptr(x) + y);

	ReconstructByD(se, ref_mask);
	this->currentImg = ref_mask - this->currentImg;
}
void Morphology::DetectConnect(Mat se, Point location)
{
	Mat x0 = Mat(this->currentImg.rows, this->currentImg.cols, CV_8U);
	x0 = 0;
	*(x0.ptr(location.x) + location.y) = 255;
	*(x0.ptr(location.x) + location.y) = 255;
	Morphology m1 = Morphology(x0);
	m1.ReconstructByD(se, this->currentImg);
	this->currentImg = m1.currentImg;
	//cv::imwrite("C:\\DIP\\week2\\output\\wire_connect.png", m1.currentImg);
}
