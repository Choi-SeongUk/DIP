
#include "CCL.h"

Mat _debug_ToColorImage(Mat img, int label_max)
{
	Mat h = Mat(img.rows, img.cols, CV_64F);
	Mat s = Mat(img.rows, img.cols, CV_64F);
	Mat i = Mat(img.rows, img.cols, CV_64F);
	s = 1;
	i = 127;
	img.convertTo(h, CV_64F);
	for (int x = 0; x < img.rows; x++)
		for (int y = 0; y < img.cols; y++)
		{
			double hue = *(h.ptr<double>(x) + y);
			double h_new;
			if (hue >= 1)
			{
				if (label_max <= 2)
				{
					*(h.ptr<double>(x) + y) = 0;
					continue;
				}
				h_new = (hue - 1) / (label_max - 1 - 1);
				h_new = h_new * (2 * pi - 0.4);
				*(h.ptr<double>(x) + y) = h_new;

				if (hue >= 255)
				{
					*(h.ptr<double>(x) + y) = 0;
					*(s.ptr<double>(x) + y) = 0;
					*(i.ptr<double>(x) + y) = 255;
				}
			}
			else if (hue < 1)
			{
				*(s.ptr<double>(x) + y) = 0;
				*(i.ptr<double>(x) + y) = 0;
			}
		}

	HSI_MAT hsi = HSI_MAT(h, s, i);
	RGB_MAT rgb = hsi.ToRGB();
	return rgb.ToImage();
}

void CCL::TwoPass(int neighbor_size)
{
	
	if (!is_one_passed)
		Pass1(neighbor_size);

	vector<int> setIDList = vector<int>(label_max);
	vector<set<int>> setList;
	
	int setNum = 0;
	set<int> s;
	s.insert(0);
	setList.push_back(s);
	setIDList[0] = setNum++;

	for (int x = 0; x < LabelImg.rows; x++)
		for (int y = 0; y < LabelImg.cols; y++)
		{
			int currentLabel = *(LabelImg.ptr<int>(x) + y);
			if (!currentLabel)
				continue;
			/*find neighbor's set*/
			for (int y_index = -1; y_index <= 1; y_index++)
				for (int x_index = -1; x_index <= 1; x_index++)
				{
					if(abs(x_index) + abs(y_index) > neighbor_size)
						continue;
					int x_neighb = x + x_index;
					int y_neighb = y + y_index;
					if (x_neighb < 0 || y_neighb < 0 || x_neighb >= LabelImg.rows || y_neighb >= LabelImg.cols)
						continue;
					int neighborLabel = *(LabelImg.ptr<int>(x_neighb) + y_neighb);
					if (neighborLabel == currentLabel)
						continue;
					if (setIDList[neighborLabel])
					{
						if (!setIDList[currentLabel])
						{
							setIDList[currentLabel] = setIDList[neighborLabel];
							setList[setIDList[neighborLabel]].insert(currentLabel);
						}
						else
						{
							if (setIDList[currentLabel] > setIDList[neighborLabel])
							{
								set<int> s = setList[setIDList[currentLabel]];
								setList[setIDList[currentLabel]].clear();
								for (set<int>::iterator iter = s.begin(); iter != s.end(); ++iter)
								{
									setIDList[*iter] = setIDList[neighborLabel];
									setList[setIDList[neighborLabel]].insert(*iter);
								}
							}
						
							else if(setIDList[currentLabel] < setIDList[neighborLabel])
							{
								set<int> s = setList[setIDList[neighborLabel]];
								setList[setIDList[neighborLabel]].clear();
								for (set<int>::iterator iter = s.begin(); iter != s.end(); ++iter)
								{
									setIDList[*iter] = setIDList[currentLabel];
									setList[setIDList[currentLabel]].insert(*iter);
								}
							}
						}
					//	goto unloop;
					}
				}
		unloop:
			if (!setIDList[currentLabel])
			{
				set<int> s;
				s.insert(currentLabel);
				setList.push_back(s);
				setIDList[currentLabel] = setNum++;
			}
		}

	
	vector<int> orderedSetIDList = vector<int>(label_max);
	int order = 1;
	for (int i = 1; i < setList.size(); i++)
	{
		set<int> s = setList[i];
		if (s.size())
		{
			for (set<int>::iterator iter = s.begin(); iter != s.end(); ++iter)
			{
				orderedSetIDList[*iter] = order;
			}
			order++;
		}
	}
	/*
	cout << "setlist" << endl;
	for (int i = 0; i < setList.size(); i++)
	{
		set<int> s = setList[i];
		if (s.size())
		{
			cout << i << "\t";

			for (set<int>::iterator iter = s.begin(); iter != s.end(); ++iter)
			{
				cout << *iter << " ";
			}
			cout << endl;
		}
		else
		{
			cout << i << "\tnone" << endl;
		}
	}

	cout << "set id list " << endl;
	for (int i = 0; i < setIDList.size(); i++)
	{
		cout << i << "¹ø: ";
		cout << setIDList[i] << " ";
		cout << endl;
	}
	cout << "order set id list " << endl;
	for (int i = 0; i < orderedSetIDList.size(); i++)
	{
		cout << i << "¹ø: ";
		cout << orderedSetIDList[i] << " ";
		cout << endl;
	}
	*/

	Mat _debugImg;
	LabelImg.convertTo(_debugImg, CV_32S);
	Mat _debugColored;
	_debugColored = _debug_ToColorImage(_debugImg, 8);
	for (int x = 0; x < originImg.rows; x++)
		for (int y = 0; y < originImg.cols; y++)
		{	
			if (*(LabelImg.ptr<int>(x) + y))
			{
				*(LabelImg.ptr<int>(x) + y) = orderedSetIDList[*(LabelImg.ptr<int>(x) + y)];
			//	if (*(_debugImg.ptr<int>(x) + y) != *(LabelImg.ptr<int>(x) + y))
				//{
				//	*(_debugImg.ptr<int>(x) + y) = *(LabelImg.ptr<int>(x) + y);
				//	_debugColored = _debug_ToColorImage(_debugImg, 8);
				//}
			//	*(_debugImg.ptr<int>(x) + y) = *(LabelImg.ptr<int>(x) + y);
			
				
			}
		}

	this->label_max = order;
	
}


void CCL::Pass1(int neighbor_size)
{
	//Mat _debugImg;
	//originImg.convertTo(_debugImg,CV_32S);
	LabelImg = Mat(originImg.rows, originImg.cols, CV_32S);
	LabelImg = 0;
	int current_label = 1;
	//Mat _debugColored;
	for (int x = 0; x < originImg.rows; x++)
		for (int y = 0; y < originImg.cols; y++)
		{
			if (*(originImg.ptr<uchar>(x) + y))
			{
				//_debugColored = _debug_ToColorImage(_debugImg, 6);

				bool is_labeled = false;
				for (int y_index = -1; y_index <= 1; y_index++)
				for (int x_index = -1; x_index <= 1; x_index++)					
					{
						if (abs(x_index) + abs(y_index) > neighbor_size)
							continue;
						int x_neighb = x + x_index;
						int y_neighb = y + y_index;
						if (x_neighb < 0 || y_neighb < 0 || x_neighb >= originImg.rows || y_neighb >= originImg.cols)
							continue;
						uchar neighbor_pixel = *(originImg.ptr<uchar>(x_neighb) + y_neighb);
						int neighbor_label = *(LabelImg.ptr<int>(x_neighb) + y_neighb);

						if (neighbor_pixel && neighbor_label) //get neigborhood's label
						{
							*(LabelImg.ptr<int>(x) + y) = neighbor_label;
							//*(_debugImg.ptr<int>(x) + y) = *(LabelImg.ptr<int>(x) + y);
							is_labeled = true;
							break;
							//y_index = 3;
							//goto unloop;
						}
					}
			unloop:
				if (!is_labeled)
				{
					*(LabelImg.ptr<int>(x) + y) = current_label++;
					//*(_debugImg.ptr<int>(x) + y) = *(LabelImg.ptr<int>(x) + y);
				}

			}
		}
		//_debugColored = _debug_ToColorImage(_debugImg, 6);
		this->label_max = current_label;
		is_one_passed = true;
}
void CCL::PassAtOnce2(int neighbor_size)
{
	//Mat _debugImg;
	//originImg.convertTo(_debugImg,CV_32S);
	//Mat _debugColored;
	//_debugColored = _debug_ToColorImage(_debugImg, 7);

	LabelImg = Mat(originImg.rows, originImg.cols, CV_32S);
	LabelImg = 0;
	queue<Point> q;
	int current_label = 1;
	for (int x = 0; x < originImg.rows; x++)
		for (int y = 0; y < originImg.cols; y++)
		{
			int* labeled = LabelImg.ptr<int>(x) + y;
			if (*(originImg.ptr<uchar>(x) + y) && !(*labeled))
			{
				*labeled = current_label;
				//	*(_debugImg.ptr<int>(x) + y) = current_label;
				//	_debugColored = _debug_ToColorImage(_debugImg, 7);

				q.push(Point(x, y));
				while (!q.empty())
				{
					Point q_pt = q.front();
					q.pop();
					int neigbor_index = (neighbor_size + 1) / 2;
					for (int x_index = -neigbor_index; x_index <= neigbor_index; x_index++)
						for (int y_index = -neigbor_index; y_index <= neigbor_index; y_index++)
						{
							uchar* current_pixel = 0;
							int* label = 0;
							if (abs(x_index) + abs(y_index) > neighbor_size)
								continue;
							int x_neighb = q_pt.x + x_index;
							int y_neighb = q_pt.y + y_index;
							if (x_neighb < 0 || y_neighb < 0 || x_neighb >= originImg.rows || y_neighb >= originImg.cols)
								continue;
							current_pixel = (originImg.ptr<uchar>(x_neighb) + y_neighb);
							label = (LabelImg.ptr<int>(x_neighb) + y_neighb);

							if (*current_pixel && !*label)//foreground and not labelled
							{
								*label = current_label;
								//*(_debugImg.ptr<int>(x_neighb) + y_neighb) = current_label;
							//	_debugColored = _debug_ToColorImage(_debugImg, 7);

								q.push(Point(x_neighb, y_neighb));
							}
						}
				}
				current_label++;
			}
		}
	this->label_max = current_label;
	is_one_passed = true;
}

void CCL::PassAtOnce(int neighbor_size)
{
	//Mat _debugImg;
	//originImg.convertTo(_debugImg,CV_32S);
	//Mat _debugColored;
	//_debugColored = _debug_ToColorImage(_debugImg, 7);

	LabelImg = Mat(originImg.rows, originImg.cols, CV_32S);
	LabelImg = 0;
	queue<Point> q;
	int current_label = 1;
	for (int x = 0; x < originImg.rows; x++)
		for (int y = 0; y < originImg.cols; y++)
		{
			int* labeled = LabelImg.ptr<int>(x) + y;
			if (*(originImg.ptr<uchar>(x) + y) && !(*labeled))
			{
				*labeled = current_label;
				//	*(_debugImg.ptr<int>(x) + y) = current_label;
				//	_debugColored = _debug_ToColorImage(_debugImg, 7);

				q.push(Point(x, y));
				while (!q.empty())
				{
					Point q_pt = q.front();
					q.pop();

					for (int x_index = -1; x_index <= 1; x_index++)
						for (int y_index = -1; y_index <= 1; y_index++)
						{
							uchar* current_pixel = 0;
							int* label = 0;
							if (abs(x_index) + abs(y_index) > neighbor_size)
								continue;
							int x_neighb = q_pt.x + x_index;
							int y_neighb = q_pt.y + y_index;
							if (x_neighb < 0 || y_neighb < 0 || x_neighb >= originImg.rows || y_neighb >= originImg.cols)
								continue;
							current_pixel = (originImg.ptr<uchar>(x_neighb) + y_neighb);
							label = (LabelImg.ptr<int>(x_neighb) + y_neighb);

							if (*current_pixel && !*label)//foreground and not labelled
							{
								*label = current_label;
								//*(_debugImg.ptr<int>(x_neighb) + y_neighb) = current_label;
							//	_debugColored = _debug_ToColorImage(_debugImg, 7);

								q.push(Point(x_neighb, y_neighb));
							}
						}
				}
				current_label++;
			}
		}
	this->label_max = current_label;
	is_one_passed = true;
}

Mat CCL::ToNumberLabeling()
{
	Mat numbedImg = this->ToColorImage();

	//originImg.copyTo(numbedImg);
	vector<int> count = vector<int>(label_max);
	vector<Point> minPoint = vector<Point>(label_max);
	vector<Point> maxPoint = vector<Point>(label_max);
	int numLabelCount = 0;
	for (int i = 1; i < label_max; i++)
	{
		minPoint[i] = Point(10000,10000);
		maxPoint[i] = Point(0, 0);
	}
	for (int x = 0; x < LabelImg.rows; x++)
		for (int y = 0; y < LabelImg.cols; y++)
		{
			int labelNum = *(LabelImg.ptr<int>(x) + y);
			if (labelNum)
			{
				count[labelNum]++;
				if (x < minPoint[labelNum].x)
					minPoint[labelNum].x = x;
				if (x > maxPoint[labelNum].x)
					maxPoint[labelNum].x = x;
				if (y < minPoint[labelNum].y)
					minPoint[labelNum].y = y;
				if (y > maxPoint[labelNum].y)
					maxPoint[labelNum].y = y;
			}
		}
	for (int i = 1; i < label_max; i++)
	{
		if (count[i] > 1)
		{
			numLabelCount++;

			minPoint[i] = minPoint[i] - Point(1, 1);
			maxPoint[i] = maxPoint[i] + Point(1, 1);
			

			rectangle(numbedImg, Point(minPoint[i].y, minPoint[i].x), Point(maxPoint[i].y, maxPoint[i].x), Scalar(255, 255, 255), 1);
			if (minPoint[i].y < 0)
				minPoint[i].y = 0;
			if (minPoint[i].x < 0)
				minPoint[i].x = 0;

			if (maxPoint[i].x >= LabelImg.rows)
				maxPoint[i].x = LabelImg.rows - 1;
			if (maxPoint[i].y >= LabelImg.cols)
				maxPoint[i].y = LabelImg.cols - 1;
			int myFontFace = 1;
			double myFontScale = 1;
			if (count[i] > 1)
			{
				//if(count[i] == 5)
			//	cv::putText(numbedImg, to_string(count[i]), Point(minPoint[i].y - 2, minPoint[i].x - 1), myFontFace, myFontScale, Scalar(255, 255, 255));
				//else
					//cv::putText(numbedImg, to_string(count[i]), Point(minPoint[i].y-2 , minPoint[i].x -1), myFontFace, myFontScale, Scalar(255, 255, 255));

			}
			else
				cv::putText(numbedImg, to_string(count[i]), Point(minPoint[i].y + 2, minPoint[i].x - 1), myFontFace, myFontScale, Scalar(255, 255, 255));
			/*
			if (minPoint[i].x > 5 && minPoint[i].y > 5)
			{
				if (minPoint[i].y < LabelImg.cols - 10)
					cv::putText(numbedImg, to_string(count[i]), Point(minPoint[i].y - 1, minPoint[i].x - 1), myFontFace, myFontScale, Scalar(255, 255, 255));
				else
					cv::putText(numbedImg, to_string(count[i]), Point(minPoint[i].y - 10, minPoint[i].x - 1), myFontFace, myFontScale, Scalar(255, 255, 255));
			}
			else
			{
				if(maxPoint[i].x >= LabelImg.rows - 10)
					cv::putText(numbedImg, to_string(count[i]), Point(maxPoint[i].y - 43, minPoint[i].x + 12), myFontFace, myFontScale, Scalar(255, 255, 255));
				else
					cv::putText(numbedImg, to_string(count[i]), Point(minPoint[i].y + 1, maxPoint[i].x + 1), myFontFace, myFontScale, Scalar(255, 255, 255));

			}
			*/
		}
	}
	cout << "¹ßÀÚ±¹ °¹¼ö: " << numLabelCount << endl;
	cout << "ÃÑ set °¹¼ö: " << this->label_max - 1 << endl;
	return numbedImg;
}
Mat CCL::ToColorImage()
{
	Mat h = Mat(LabelImg.rows, LabelImg.cols, CV_64F);
	Mat s = Mat(LabelImg.rows, LabelImg.cols, CV_64F);
	Mat i = Mat(LabelImg.rows, LabelImg.cols, CV_64F);
	s = 1;
	i = 127;
	LabelImg.convertTo(h, CV_64F);
	for (int x = 0; x < LabelImg.rows; x++)
		for (int y = 0; y < LabelImg.cols; y++)
		{
			double hue = *(h.ptr<double>(x) + y);
			double h_new;
			if (hue >= 1)
			{
				if (label_max <= 2)
				{
					*(h.ptr<double>(x) + y) = 0;
					continue;
				}
				h_new = (hue - 1) / (label_max - 1 - 1);
				h_new = h_new * (2 * pi - 0.4);
				*(h.ptr<double>(x) + y) = h_new;
			}
			else if (hue < 1)
			{
				*(s.ptr<double>(x) + y) = 0;
				*(i.ptr<double>(x) + y) = 0;
			}
		}

	HSI_MAT hsi = HSI_MAT(h, s, i);
	RGB_MAT rgb = hsi.ToRGB();
	return rgb.ToImage();
}
CCL::CCL()
{
}

CCL::CCL(Mat image)
{
	this->originImg = image;
}


CCL::~CCL()
{
}
