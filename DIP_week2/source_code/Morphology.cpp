#include "Morphology.h"


void Morphology::Erosion(Mat mask)
{
	Mat newImg = Mat(this->currentImg.rows,this->currentImg.cols,CV_8U);
	newImg = Scalar(0);
	for (int x = 0; x < this->currentImg.rows; x++)
		for (int y = 0; y < this->currentImg.cols; y++)
		{
			bool erose = false;
			for (int i = -mask.rows / 2; i < mask.rows / 2 + 1; i++)
				for (int j = -mask.cols / 2; j < mask.cols / 2 + 1; j++)
				{
					
					int image_pixel;
					if (y + j < 0 || y + j >= this->currentImg.cols)
						image_pixel = high;
					else if (x + i < 0 || x + i >= this->currentImg.rows)
						image_pixel = high;
					else
						image_pixel = *(this->currentImg.ptr<uchar>(x + i) + y + j);
					int mask_pixel = *(mask.ptr<uchar>(i + mask.rows / 2) + j + mask.cols / 2);
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
void Morphology::Dilation(Mat mask)
{
	Mat newImg = Mat(this->currentImg.rows, this->currentImg.cols, CV_8U);
	newImg = Scalar(0);
	for (int x = 0; x < this->currentImg.rows; x++)
		for (int y = 0; y < this->currentImg.cols; y++)
		{
			bool dilation = false;
			for (int i = -mask.rows / 2; i < mask.rows / 2 + 1; i++)
				for (int j = -mask.cols / 2; j < mask.cols / 2 + 1; j++)
				{
					int image_pixel;
					if (y + j < 0 || y + j >= this->currentImg.cols)
						image_pixel = low;
					else if (x + i < 0 || x + i >= this->currentImg.rows)
						image_pixel = low;
					else
						image_pixel = *(this->currentImg.ptr<uchar>(x + i) + y + j);
					int mask_pixel = *(mask.ptr<uchar>(-i + mask.rows / 2) - j + mask.cols / 2);
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
Morphology::Morphology(Mat in)
{
	in.copyTo(currentImg);
}


