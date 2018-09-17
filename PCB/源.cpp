
//----------------------------【头文件，命名空间和包含部分】----------------------------------------------------------
// 描述：包含程序所依赖的头文件和命名空间
//--------------------------------------------------------------------------------------
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;

//-------------------------【宏定义部分】-------------------------------------------------------------
 //描述：定义辅助宏
//--------------------------------------------------------------------------------------
#define W_NAME1 "程序窗口1"            //为窗口定义的标题
#define W_NAME2 "程序窗口2"

//-------------------------【全局变量声明部分】-------------------------------------------------------------
//描述：全局变量声明
//--------------------------------------------------------------------------------------
Mat g_srcImage, g_srcImage1,g_grayImage;
int thresh = 30;//当前阈值
int max_thresh = 175;//最大阈值

//--------------------------【全局函数数声明部分】------------------------------------------------------------
//描述：全局函数声明
//--------------------------------------------------------------------------------------
void on_CornerHarris(int, void*);//回调函数
//--------------------------【main（）函数】------------------------------------------------------------
//描述：控制台应用程序的入口函数，
//--------------------------------------------------------------------------------------
int main(int argc, char**argv)
{
	//【1】载入原始图并进行克隆保存
	g_srcImage = imread("1 (2).jpg", 1);
	if (!g_srcImage.data)
	{
		cout << " 载入原始图错误";
		return false;
	}
	imshow("原始图", g_srcImage);
	g_srcImage1 = g_srcImage.clone();

	//【2】存留一张灰度图
	cvtColor(g_srcImage1, g_grayImage, COLOR_BGR2GRAY);
	//【3】创建窗口和滚动条
	namedWindow(W_NAME1, WINDOW_AUTOSIZE);
	createTrackbar("阈值", W_NAME1, &thresh, max_thresh, on_CornerHarris);
	//【4】调用回调函数并进行初始化
	on_CornerHarris(0, 0);

	waitKey(0);
	return 0;
}
//--------------------------【on_HoughLines函数】------------------------------------------------------------
//描述 回调函数
//--------------------------------------------------------------------------------------
void on_CornerHarris(int, void *)
{
	//【1】定义局部变量
	Mat i_dstImage;//目标图
	Mat i_normImage;//归一化的效果图
	Mat i_scaledImage;//线性变换和的8位无符号整型图

	//【2】初始化
	//置零当前需要显示的两幅图，即清除上一次调用此函数时的值
	i_dstImage = Mat::zeros(g_srcImage.size(), CV_32FC1);
	g_srcImage1 = g_srcImage.clone();
	//【3】正式检测
	//进行角点检测
	cornerHarris(g_grayImage, i_dstImage, 2, 3, 0.04, BORDER_DEFAULT);
	//归一化与转换
	normalize(i_dstImage, i_normImage, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	convertScaleAbs(i_normImage, i_scaledImage);//将归一化后的图线性变换成8位的无符号整型
	//【4】进行绘制
	//将检测到的，且符合阈值条件的角点绘制出来
	for (int j = 0; j < i_normImage.rows; j++)
	{
		for (int i = 0; i < i_normImage.cols; i++)
		{
			if ((int)i_normImage.at<float>(j, i) > thresh + 80)
			{
				circle(g_srcImage1, Point(i, j), 5,Scalar(10, 10, 255), 2, 8, 0);
				circle(i_scaledImage, Point(i, j), 5, Scalar(0, 10, 255), 2, 8, 0);
			}
		}
	}
	//【5】显示最终效果
	imshow(W_NAME1, g_srcImage1);
	imshow(W_NAME2, i_scaledImage);

}