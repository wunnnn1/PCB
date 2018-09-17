//#include "stdafx.h"
/*
minAreaRect()���Ի����ת�ľ���
��Ҫ�洢��RotatedRect�У�Box2D



*/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main()
{
	//�ԻҶ�ģʽ���벢��ʾ
	Mat srcImg = imread("2.jpg", 0);
	imshow("ԭʼͼ",srcImg);

	//����Harris �ǵ����ҳ��ǵ�
	Mat cornerStrength;
	cornerHarris(srcImg, cornerStrength, 2, 3, 0.01);

	//�ԻҶ�ͼ�������ֵ�������õ���ֵͼ����ʾ
	Mat harrisCorner;
	threshold(cornerStrength, harrisCorner, 0.00001, 255, THRESH_BINARY);
	imshow("�ǵ����Ķ�ֵЧ��ͼ", harrisCorner);
	waitKey(0);
	return 0;


}
