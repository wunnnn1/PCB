//#include "stdafx.h"
/*
minAreaRect()可以获得旋转的矩形
需要存储在RotatedRect中，Box2D



*/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main()
{
	//以灰度模式载入并显示
	Mat srcImg = imread("2.jpg", 0);
	imshow("原始图",srcImg);

	//进行Harris 角点检测找出角点
	Mat cornerStrength;
	cornerHarris(srcImg, cornerStrength, 2, 3, 0.01);

	//对灰度图像进行阈值操做，得到二值图并显示
	Mat harrisCorner;
	threshold(cornerStrength, harrisCorner, 0.00001, 255, THRESH_BINARY);
	imshow("角点检测后的二值效果图", harrisCorner);
	waitKey(0);
	return 0;


}
