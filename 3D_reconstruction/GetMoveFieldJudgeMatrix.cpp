#include<opencv2/opencv.hpp>
//#include <pcl/point_types.h>				//点类型定义
//#include <pcl/point_cloud.h>				//点云类定义
//#include <pcl/visualization/cloud_viewer.h>                //点云显示

using namespace std;
using namespace cv;


Mat GetMoveFieldJudgeMatrix(vector<Mat>pictures)
{

	//创建移动区域判断矩阵，CV_16UC1是为了防止数据溢出
	Mat moveFieldJudgeMatrix = Mat::zeros(pictures[0].size(), CV_16UC1);

	//帧间差分法，图片两两相减
	for (int pictureNum = 0; pictureNum != (pictures.size() - 1); pictureNum++)
	{
		//由彩色图像转换成灰度图像
		Mat previousImg;

		cout << "颜色值：" << pictures[pictureNum].at<Vec3b>(2124.5, 2950.5) << endl;

		cvtColor(pictures[pictureNum], previousImg, CV_RGB2GRAY);

		cout << "前一张图的（0,0）处的像素值：" << (int)previousImg.at<uchar>(0, 0) << endl;

		Mat nextImg;
		cvtColor(pictures[pictureNum + 1], nextImg, CV_RGB2GRAY);

		cout << "后一张图的（0,0）处的像素值：" << (int)nextImg.at<uchar>(0, 0) << endl;

		Mat differImg = abs(previousImg - nextImg);

		cout << "相减后得到的图的（0,0）处的像素值：" << (int)differImg.at<uchar>(0, 0) << endl;

		//转变成CV_16UC1,否则相加时会导致一溢出
		differImg.convertTo(differImg, CV_16UC1);

		cout << "相减后得到的图经过数据类型转换后图的（0,0）处的像素值：" << (int)differImg.at<unsigned short>(0, 0) << endl;

		moveFieldJudgeMatrix += differImg;

		cout << endl << endl << endl;
		cout << "下一次相减" << endl;
	}

	//以热力图显示
	//normalize(moveFieldJudgeMatrix, moveFieldJudgeMatrix, 0, 256, NORM_MINMAX, -1, Mat());
	//moveFieldJudgeMatrix.convertTo(moveFieldJudgeMatrix, CV_8UC1);
	//applyColorMap(moveFieldJudgeMatrix, moveFieldJudgeMatrix, COLORMAP_JET);
	//namedWindow("Otsu", CV_WINDOW_NORMAL);
	//imshow("Otsu", moveFieldJudgeMatrix);
	//waitKey(0);

	/*
	根据阈值进行二值化
	*/
	//取得moveFieldJudgeMatrix的最大值
	double max, min;
	Point min_loc, max_loc;
	minMaxLoc(moveFieldJudgeMatrix, &min, &max, &min_loc, &max_loc);


	////归一化
	//normalize(moveFieldJudgeMatrix, moveFieldJudgeMatrix, 0, 256, NORM_MINMAX, -1, Mat());
	////转换数据类型
	//moveFieldJudgeMatrix.convertTo(moveFieldJudgeMatrix, CV_8UC1);

	//namedWindow("Otsu", CV_WINDOW_NORMAL);
	//imshow("Otsu", moveFieldJudgeMatrix);
	//waitKey(0);

	////根据OTSU（大津算法）找到阈值进行二值化
	//double Otsu = 0;
	//Otsu = threshold(moveFieldJudgeMatrix, moveFieldJudgeMatrix, Otsu, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
	//cout << "OpenCVthresh=" << Otsu << endl;

	//namedWindow("Otsu", CV_WINDOW_NORMAL);
	//imshow("Otsu", moveFieldJudgeMatrix);
	//waitKey(0);

	//自设定阈值进行二值化
	for (int row = 0; row < moveFieldJudgeMatrix.rows; ++row)
	{
		for (int col = 0; col < moveFieldJudgeMatrix.cols; ++col)
		{
			if (moveFieldJudgeMatrix.at<unsigned short>(row, col) < 160)
			{
				moveFieldJudgeMatrix.at<unsigned short>(row, col) = 0;
			}
			else
			{
				moveFieldJudgeMatrix.at<unsigned short>(row, col) = 255;
			}
		}
	}

	//转换成CV_8UC1，方便使用imshow
	moveFieldJudgeMatrix.convertTo(moveFieldJudgeMatrix, CV_8UC1);

	return moveFieldJudgeMatrix;
}
