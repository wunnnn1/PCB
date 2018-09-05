//#include "stdafx.h"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
	VideoCapture cap;
	cap = VideoCapture(CV_CAP_DSHOW + 0);	//ʹ��DirectShow    +1����1������ͷĬ����0��
	if (!cap.isOpened())
	{
		cout << "Can't open your camera." << endl;
		return -1;
	}
	cap.set(CV_CAP_PROP_FOURCC, CV_FOURCC('Y', 'U', 'V', '2'));//����ΪMJPG��ʽ
															   //	cap.set(CV_CAP_PROP_EXPOSURE, -7);//�����ع�ʱ��
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1920);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 1080);
	while (1)
	{
		Mat img;
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
		imshow("img", img);
		waitKey(1);
	}

	return 0;
}


