//#include "stdafx.h"
/*
minAreaRect()可以获得旋转的矩形
需要存储在RotatedRect中，Box2D



*/
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;
int wid = 8; //每一帧长的十分之一作为单位矩形的长
int len = 6; //每一帧宽十分之一作为单位矩形的宽
int iLowH = 19;
int iHighH = 98;
int iLowS = 178;
int iHighS = 255;
int iLowV = 0;
int iHighV = 203;
//面积比28:1

Mat frames, hsv;
Mat temp_frame;
/*
得到想要追踪的目标，将图像二值化
我们想要的目标像素值给255，其他背景给0
*/
void HSV_(Mat& frame, Mat& dst)
{
	Mat frameHSV;
	dst = frame.clone();
	std::vector<Mat> splited;
	cvtColor(frame, frameHSV, COLOR_BGR2HSV);
	split(frameHSV, splited);
	equalizeHist(splited[2], splited[2]);
	merge(splited, frameHSV);
	inRange(frameHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), dst);
	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
	morphologyEx(dst, dst, MORPH_OPEN, element);
	morphologyEx(dst, dst, MORPH_CLOSE, element);
}
Mat Contours_r(Mat inrange, RotatedRect &maxRect)//旋转的矩形
{
	Mat  open, close;
	int area[1010], i;
	int areamax;
	Mat Ctimg;//连通域图
	stringstream ss;

	/*形态学操作*/
	Mat element = getStructuringElement(MORPH_RECT, Size(2, 2));
	morphologyEx(inrange, close, MORPH_CLOSE, element);
	morphologyEx(close, open, MORPH_OPEN, element);

	/*轮廓寻找与面积的计算*/
	vector<Point> points;
	vector<vector<cv::Point>> contours;
	/*
			cv::很重要，不然可能出问题:原因是标准库里有std::vector 和 Point 
		和findContours里要用到的vector和Point不是一回事所以，声明的时候要用
		cv::vector和cv::Point就可以了
	*/
	vector<Vec4i> hierarchy;
	findContours(open, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	double maxArea = 0;
	for (long i = 0; i < contours.size(); i++)
	{
		double each = contourArea(contours[i]);
		if (each > maxArea)
		{
			maxArea = each;
			points = contours[i];
		}
	}
	if(points.size())
	maxRect = minAreaRect(points);

	//if (maxRect.size.area()> 50)//加上判断，防止在丢失目标时还画框
	if (1)//加上判断，防止在丢失目标时还画框
		{
		rectangle(open, maxRect.boundingRect(), Scalar(100));
		ss << "__at" << maxRect.center<<"size"<<maxRect.size;
		putText(open,        //目标图片
			ss.str(),     //待绘制的文字
			maxRect.center,//左下角
			FONT_HERSHEY_COMPLEX_SMALL, //字体
			1,//尺寸因子，值越大字体越大,double
			Scalar(100),
			1,//线条宽度
			4, //线型，4或8邻域
			0);//1,反转180°,0不反转
	}
	return open;
}
void simp_sun(Mat input) {
	//Mat to IplImzge 深拷贝
	IplImage imgTmp = input;
	IplImage* src = &imgTmp;


	int height = src->height;
	int width = src->width;
	int step = src->widthStep;
	int i = 0, j = 0;
	int R, G, B, A;
	unsigned char* srcData;

	for (i = 0; i < height; i++)
	{
		srcData = (unsigned char*)src->imageData + i * step;
		for (j = 0; j < width; j++)
		{
			R = srcData[j * 3];
			G = srcData[j * 3 + 1];
			B = srcData[j * 3 + 2];
			if ((R + G + B) > 150)
			{
				A = max(max(R, G), B);//求最大值

				srcData[j * 3 + 0] = 150 * R / (A + 1);
				srcData[j * 3 + 1] = 150 * G / (A + 1);
				srcData[j * 3 + 2] = 150 * B / (A + 1);
			}
		}
	}

}

int main()
{
	cvNamedWindow("controlRANGE");//range窗口
	cvNamedWindow("ControlHSV");//hsv窗口
								//	cvNamedWindow("manycsize");//内核
	createTrackbar("LowH", "ControlHSV", &iLowH, 255); //
	createTrackbar("HighH", "ControlHSV", &iHighH, 255);
	createTrackbar("LowS", "ControlHSV", &iLowS, 255); //
	createTrackbar("HighS", "ControlHSV", &iHighS, 255);
	createTrackbar("LowV", "ControlHSV", &iLowV, 255); //
	createTrackbar("HighV", "ControlHSV", &iHighV, 255);
	VideoCapture cap;
	cap = VideoCapture(CV_CAP_DSHOW + 0);	//使用DirectShow    +1则是1号摄像头默认是0号
	if (!cap.isOpened())
	{
		cout << "Can't open your camera." << endl;
		return -1;
	}
	//cap.set(CV_CAP_PROP_FOURCC, CV_FOURCC('Y', 'U', 'V', '2'));//设置为MJPG格式
															   //	cap.set(CV_CAP_PROP_EXPOSURE, -7);//设置曝光时间
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1920);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 1080);
	while (1)
	{
		Mat img;
		RotatedRect box;
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
		HSV_(img, hsv);
		frames=Contours_r(hsv, box);
		imshow("img", img);
		imshow("hsv", hsv);
		imshow("contours", frames);
		waitKey(1);
		/*转化为HSV求连通域*/

		/*
		遍历循环，输出点的坐标
		输出点面积
		输出形状
		*/

	}

	return 0;
}


