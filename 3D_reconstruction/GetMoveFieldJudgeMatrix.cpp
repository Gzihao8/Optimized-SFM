#include<opencv2/opencv.hpp>
//#include <pcl/point_types.h>				//�����Ͷ���
//#include <pcl/point_cloud.h>				//�����ඨ��
//#include <pcl/visualization/cloud_viewer.h>                //������ʾ

using namespace std;
using namespace cv;


Mat GetMoveFieldJudgeMatrix(vector<Mat>pictures)
{

	//�����ƶ������жϾ���CV_16UC1��Ϊ�˷�ֹ�������
	Mat moveFieldJudgeMatrix = Mat::zeros(pictures[0].size(), CV_16UC1);

	//֡���ַ���ͼƬ�������
	for (int pictureNum = 0; pictureNum != (pictures.size() - 1); pictureNum++)
	{
		//�ɲ�ɫͼ��ת���ɻҶ�ͼ��
		Mat previousImg;

		cout << "��ɫֵ��" << pictures[pictureNum].at<Vec3b>(2124.5, 2950.5) << endl;

		cvtColor(pictures[pictureNum], previousImg, CV_RGB2GRAY);

		cout << "ǰһ��ͼ�ģ�0,0����������ֵ��" << (int)previousImg.at<uchar>(0, 0) << endl;

		Mat nextImg;
		cvtColor(pictures[pictureNum + 1], nextImg, CV_RGB2GRAY);

		cout << "��һ��ͼ�ģ�0,0����������ֵ��" << (int)nextImg.at<uchar>(0, 0) << endl;

		Mat differImg = abs(previousImg - nextImg);

		cout << "�����õ���ͼ�ģ�0,0����������ֵ��" << (int)differImg.at<uchar>(0, 0) << endl;

		//ת���CV_16UC1,�������ʱ�ᵼ��һ���
		differImg.convertTo(differImg, CV_16UC1);

		cout << "�����õ���ͼ������������ת����ͼ�ģ�0,0����������ֵ��" << (int)differImg.at<unsigned short>(0, 0) << endl;

		moveFieldJudgeMatrix += differImg;

		cout << endl << endl << endl;
		cout << "��һ�����" << endl;
	}

	//������ͼ��ʾ
	//normalize(moveFieldJudgeMatrix, moveFieldJudgeMatrix, 0, 256, NORM_MINMAX, -1, Mat());
	//moveFieldJudgeMatrix.convertTo(moveFieldJudgeMatrix, CV_8UC1);
	//applyColorMap(moveFieldJudgeMatrix, moveFieldJudgeMatrix, COLORMAP_JET);
	//namedWindow("Otsu", CV_WINDOW_NORMAL);
	//imshow("Otsu", moveFieldJudgeMatrix);
	//waitKey(0);

	/*
	������ֵ���ж�ֵ��
	*/
	//ȡ��moveFieldJudgeMatrix�����ֵ
	double max, min;
	Point min_loc, max_loc;
	minMaxLoc(moveFieldJudgeMatrix, &min, &max, &min_loc, &max_loc);


	////��һ��
	//normalize(moveFieldJudgeMatrix, moveFieldJudgeMatrix, 0, 256, NORM_MINMAX, -1, Mat());
	////ת����������
	//moveFieldJudgeMatrix.convertTo(moveFieldJudgeMatrix, CV_8UC1);

	//namedWindow("Otsu", CV_WINDOW_NORMAL);
	//imshow("Otsu", moveFieldJudgeMatrix);
	//waitKey(0);

	////����OTSU������㷨���ҵ���ֵ���ж�ֵ��
	//double Otsu = 0;
	//Otsu = threshold(moveFieldJudgeMatrix, moveFieldJudgeMatrix, Otsu, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
	//cout << "OpenCVthresh=" << Otsu << endl;

	//namedWindow("Otsu", CV_WINDOW_NORMAL);
	//imshow("Otsu", moveFieldJudgeMatrix);
	//waitKey(0);

	//���趨��ֵ���ж�ֵ��
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

	//ת����CV_8UC1������ʹ��imshow
	moveFieldJudgeMatrix.convertTo(moveFieldJudgeMatrix, CV_8UC1);

	return moveFieldJudgeMatrix;
}
