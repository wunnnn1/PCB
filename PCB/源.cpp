
//准备从此继续
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
#define W_NAME1 "Shi-Tomasi 角点检测"            //为窗口定义的标题
#define W_NAME2 "程序窗口2"

//-------------------------【全局变量声明部分】-------------------------------------------------------------
//描述：全局变量声明
//--------------------------------------------------------------------------------------
Mat g_srcImage, g_grayimage;
int g_maxConreNumber = 33;
int g_maxTrackbarNumber = 500;
RNG g_rng(12345);
//--------------------------【全局函数数声明部分】------------------------------------------------------------
//描述：全局函数声明
//--------------------------------------------------------------------------------------
void on_GoodFeaturseToTrack(int, void*);
//--------------------------【main（）函数】------------------------------------------------------------
//描述：控制台应用程序的入口函数，
//--------------------------------------------------------------------------------------
int main()
{
	//【1】载入原图像并转化为灰度
	g_srcImage = imread("1 (3).jpg", 1);
	cvtColor(g_srcImage, g_grayimage, COLOR_BGR2GRAY);

	//【2】创建窗口和滑动条，并进行显示和回调函数初始化
	namedWindow(W_NAME1, WINDOW_AUTOSIZE);
	createTrackbar("最大角点数", W_NAME1, &g_maxTrackbarNumber, g_maxTrackbarNumber, on_GoodFeaturseToTrack);
	imshow(W_NAME1, g_srcImage);
	on_GoodFeaturseToTrack(0, 0);
	waitKey(0);
	return(0);
}
//--------------------------【on_GoodFeaturseToTrack_demo()函数】------------------------------------------------------------
//描述：响应滑动条移动消息的回调函数
//--------------------------------------------------------------------------------------
void on_GoodFeaturseToTrack(int, void*)
{
	//【1】对变量小于等于1时的处理
	if (g_maxConreNumber <= 1)
		g_maxConreNumber = 1;

	//【2】	Shi-Tomasi算法的参数准备（goodFeaturesToTrack函数）
	vector<Point2f>corners;
	double qualityLevel = 0.01;//角点检测可接受的最小特征值
	double minDistance = 10;//角点之间最小的距离
	int blockSize = 3;//计算导数自相关矩阵时指定的邻域范围
	double k = 0.04;//权重系数
	Mat copy = g_srcImage.clone();

	//【3】进行Shi-Tomasi角点检测
	goodFeaturesToTrack(g_grayimage,//输入
		corners,//检测到角点的输出向量
		g_maxConreNumber,//角点最大数量
		qualityLevel,//角点可接受的最小特征值
		minDistance,//角点之间的最小距离
		Mat(),//感兴趣区域
		blockSize,//计算导数自相关矩阵时指定的邻域范围
		false,//不使用Harris角点检测
		k);//权重系数
	//【4】输出文字信息
	std::cout <<"1此次检测到的角点数量为："<< corners.size() << endl;


	//【5】绘制检测到的角点
	int r = 4;
	for (unsigned int i = 0; i < corners.size(); i++)
	{
		//以随机的颜色绘制出角点
		circle(copy, corners[i], r, Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255)), -1, 8, 0);

	}
	//【6】	显示与更新窗口
	imshow(W_NAME1, copy);
}