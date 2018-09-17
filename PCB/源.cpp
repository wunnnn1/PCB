
//----------------------------��ͷ�ļ��������ռ�Ͱ������֡�----------------------------------------------------------
// ����������������������ͷ�ļ��������ռ�
//--------------------------------------------------------------------------------------
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;

//-------------------------���궨�岿�֡�-------------------------------------------------------------
 //���������帨����
//--------------------------------------------------------------------------------------
#define W_NAME1 "���򴰿�1"            //Ϊ���ڶ���ı���
#define W_NAME2 "���򴰿�2"

//-------------------------��ȫ�ֱ����������֡�-------------------------------------------------------------
//������ȫ�ֱ�������
//--------------------------------------------------------------------------------------
Mat g_srcImage, g_srcImage1,g_grayImage;
int thresh = 30;//��ǰ��ֵ
int max_thresh = 175;//�����ֵ

//--------------------------��ȫ�ֺ������������֡�------------------------------------------------------------
//������ȫ�ֺ�������
//--------------------------------------------------------------------------------------
void on_CornerHarris(int, void*);//�ص�����
//--------------------------��main����������------------------------------------------------------------
//����������̨Ӧ�ó������ں�����
//--------------------------------------------------------------------------------------
int main(int argc, char**argv)
{
	//��1������ԭʼͼ�����п�¡����
	g_srcImage = imread("1 (2).jpg", 1);
	if (!g_srcImage.data)
	{
		cout << " ����ԭʼͼ����";
		return false;
	}
	imshow("ԭʼͼ", g_srcImage);
	g_srcImage1 = g_srcImage.clone();

	//��2������һ�ŻҶ�ͼ
	cvtColor(g_srcImage1, g_grayImage, COLOR_BGR2GRAY);
	//��3���������ں͹�����
	namedWindow(W_NAME1, WINDOW_AUTOSIZE);
	createTrackbar("��ֵ", W_NAME1, &thresh, max_thresh, on_CornerHarris);
	//��4�����ûص����������г�ʼ��
	on_CornerHarris(0, 0);

	waitKey(0);
	return 0;
}
//--------------------------��on_HoughLines������------------------------------------------------------------
//���� �ص�����
//--------------------------------------------------------------------------------------
void on_CornerHarris(int, void *)
{
	//��1������ֲ�����
	Mat i_dstImage;//Ŀ��ͼ
	Mat i_normImage;//��һ����Ч��ͼ
	Mat i_scaledImage;//���Ա任�͵�8λ�޷�������ͼ

	//��2����ʼ��
	//���㵱ǰ��Ҫ��ʾ������ͼ���������һ�ε��ô˺���ʱ��ֵ
	i_dstImage = Mat::zeros(g_srcImage.size(), CV_32FC1);
	g_srcImage1 = g_srcImage.clone();
	//��3����ʽ���
	//���нǵ���
	cornerHarris(g_grayImage, i_dstImage, 2, 3, 0.04, BORDER_DEFAULT);
	//��һ����ת��
	normalize(i_dstImage, i_normImage, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	convertScaleAbs(i_normImage, i_scaledImage);//����һ�����ͼ���Ա任��8λ���޷�������
	//��4�����л���
	//����⵽�ģ��ҷ�����ֵ�����Ľǵ���Ƴ���
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
	//��5����ʾ����Ч��
	imshow(W_NAME1, g_srcImage1);
	imshow(W_NAME2, i_scaledImage);

}