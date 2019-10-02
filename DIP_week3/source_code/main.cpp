#include "main.h"

Morphology mor;
Histogram histo;
Mat gimage;
void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{

		Mat mask = Mat(3, 3, CV_8U);
		mask = 255;
		mor.HoleFill(mask, Point(x, y));

	}
	else if (event == EVENT_RBUTTONDOWN)
	{
		string img_name;
		std::cout << "저장할 파일명:";
		cin >> img_name;
		cv::imwrite("C:\\DIP\\week3\\output\\" + img_name + ".png", gimage);
		std::cout << "saved!" << endl;
	}
}

int main()
{
	string img_name = "d";
	while (img_name != "end")
	{
		cout << "파일명 입력:";
		cin >> img_name;
		cv::Mat image = cv::imread("C:\\DIP\\week3\\input\\" + img_name + ".png", cv::IMREAD_GRAYSCALE);
	
		while (1)
		{
			int neighborsize = 7;
			cout << "neigbor:";
			cin >> neighborsize;
			CCL ccl = CCL(image);
			ccl.Pass1(neighborsize);
			//ccl.TwoPass(neighborsize);
			ccl.PassAtOnce2(neighborsize);
			
			Mat image_8d = ccl.ToNumberLabeling();
		}
		
		//cv::imwrite("C:\\DIP\\week3\\output\\" + img_name + "_pass2_8.png", gimage);
		//ccl.TwoPass(1);
		
	
		cout << "saved" << endl;
	}
	
	
	


	cv::namedWindow("ImageDisplay", 1);
	cv::setMouseCallback("ImageDisplay", CallBackFunc, NULL);
	cv::imshow("ImageDisplay", gimage);
	
	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}

