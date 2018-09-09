//#include "stdafx.h"
/*
minAreaRect()���Ի����ת�ľ���
��Ҫ�洢��RotatedRect�У�Box2D



*/
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;
int wid = 8; //ÿһ֡����ʮ��֮һ��Ϊ��λ���εĳ�
int len = 6; //ÿһ֡��ʮ��֮һ��Ϊ��λ���εĿ�
int iLowH = 19;
int iHighH = 98;
int iLowS = 178;
int iHighS = 255;
int iLowV = 0;
int iHighV = 203;
//�����28:1

Mat frames, hsv;
Mat temp_frame;
/*
�õ���Ҫ׷�ٵ�Ŀ�꣬��ͼ���ֵ��
������Ҫ��Ŀ������ֵ��255������������0
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
Mat Contours_r(Mat inrange, RotatedRect &maxRect)//��ת�ľ���
{
	Mat  open, close;
	int area[1010], i;
	int areamax;
	Mat Ctimg;//��ͨ��ͼ
	stringstream ss;

	/*��̬ѧ����*/
	Mat element = getStructuringElement(MORPH_RECT, Size(2, 2));
	morphologyEx(inrange, close, MORPH_CLOSE, element);
	morphologyEx(close, open, MORPH_OPEN, element);

	/*����Ѱ��������ļ���*/
	vector<Point> points;
	vector<vector<cv::Point>> contours;
	/*
			cv::����Ҫ����Ȼ���ܳ�����:ԭ���Ǳ�׼������std::vector �� Point 
		��findContours��Ҫ�õ���vector��Point����һ�������ԣ�������ʱ��Ҫ��
		cv::vector��cv::Point�Ϳ�����
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

	//if (maxRect.size.area()> 50)//�����жϣ���ֹ�ڶ�ʧĿ��ʱ������
	if (1)//�����жϣ���ֹ�ڶ�ʧĿ��ʱ������
		{
		rectangle(open, maxRect.boundingRect(), Scalar(100));
		ss << "__at" << maxRect.center<<"size"<<maxRect.size;
		putText(open,        //Ŀ��ͼƬ
			ss.str(),     //�����Ƶ�����
			maxRect.center,//���½�
			FONT_HERSHEY_COMPLEX_SMALL, //����
			1,//�ߴ����ӣ�ֵԽ������Խ��,double
			Scalar(100),
			1,//�������
			4, //���ͣ�4��8����
			0);//1,��ת180��,0����ת
	}
	return open;
}
void simp_sun(Mat input) {
	//Mat to IplImzge ���
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
				A = max(max(R, G), B);//�����ֵ

				srcData[j * 3 + 0] = 150 * R / (A + 1);
				srcData[j * 3 + 1] = 150 * G / (A + 1);
				srcData[j * 3 + 2] = 150 * B / (A + 1);
			}
		}
	}

}

int main()
{
	cvNamedWindow("controlRANGE");//range����
	cvNamedWindow("ControlHSV");//hsv����
								//	cvNamedWindow("manycsize");//�ں�
	createTrackbar("LowH", "ControlHSV", &iLowH, 255); //
	createTrackbar("HighH", "ControlHSV", &iHighH, 255);
	createTrackbar("LowS", "ControlHSV", &iLowS, 255); //
	createTrackbar("HighS", "ControlHSV", &iHighS, 255);
	createTrackbar("LowV", "ControlHSV", &iLowV, 255); //
	createTrackbar("HighV", "ControlHSV", &iHighV, 255);
	VideoCapture cap;
	cap = VideoCapture(CV_CAP_DSHOW + 0);	//ʹ��DirectShow    +1����1������ͷĬ����0��
	if (!cap.isOpened())
	{
		cout << "Can't open your camera." << endl;
		return -1;
	}
	//cap.set(CV_CAP_PROP_FOURCC, CV_FOURCC('Y', 'U', 'V', '2'));//����ΪMJPG��ʽ
															   //	cap.set(CV_CAP_PROP_EXPOSURE, -7);//�����ع�ʱ��
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

		putText(img,        //Ŀ��ͼƬ
			text.str(),     //�����Ƶ�����
			Point(100, 100),//���½�
			FONT_HERSHEY_COMPLEX_SMALL, //����
			1.1,//�ߴ����ӣ�ֵԽ������Խ��,double
			Scalar(255, 100, 234),
			3,//�������
			4, //���ͣ�4��8����
			0);//1,��ת180��,0����ת
		HSV_(img, hsv);
		frames=Contours_r(hsv, box);
		imshow("img", img);
		imshow("hsv", hsv);
		imshow("contours", frames);
		waitKey(1);
		/*ת��ΪHSV����ͨ��*/

		/*
		����ѭ��������������
		��������
		�����״
		*/

	}

	return 0;
}


