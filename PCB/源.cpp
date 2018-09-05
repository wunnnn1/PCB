//#include "stdafx.h"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
	VideoCapture cap;
	cap = VideoCapture(CV_CAP_DSHOW + 0);	//使用DirectShow    +1则是1号摄像头默认是0号
	if (!cap.isOpened())
	{
		cout << "Can't open your camera." << endl;
		return -1;
	}
	cap.set(CV_CAP_PROP_FOURCC, CV_FOURCC('Y', 'U', 'V', '2'));//设置为MJPG格式
															   //	cap.set(CV_CAP_PROP_EXPOSURE, -7);//设置曝光时间
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1920);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 1080);
	while (1)
	{
		Mat img;
		cap >> img;
		stringstream text;
		int i = 100;
		text << "area" << i;

		putText(img,        //目标图片
			text.str(),     //待绘制的文字
			Point(100, 100),//左下角
			FONT_HERSHEY_COMPLEX_SMALL, //字体
			1.1,//尺寸因子，值越大字体越大,double
			Scalar(255, 100, 234),
			3,//线条宽度
			4, //线型，4或8邻域
			0);//1,反转180°,0不反转
		imshow("img", img);
		waitKey(1);
	}

	return 0;
}


