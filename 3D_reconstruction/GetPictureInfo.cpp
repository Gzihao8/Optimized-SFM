#include<opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include"MyDataStructure.h"

using namespace std;
using namespace cv;

PictureInfo GetPictureInfo(Mat img)
{
	PictureInfo pictureInfo;

	Ptr<Feature2D> surf = xfeatures2d::SURF::create();

	vector<Vec3b> pointsColors;

	//��ȡ������
	surf->detect(img,pictureInfo.keyPoints);
	//����������������
	surf->compute(img,pictureInfo.keyPoints,pictureInfo.descriptors);
	//��ȡ�����㴦��BGRֵ
	pictureInfo.pointsColors.clear();
	for (int i = 0; i < pictureInfo.keyPoints.size();i++)
	{
		//��ȡBGRֵʱ�����꣬��Ϊopencv��ͼ������ϵ�ǣ�
		//ԭ�������Ͻǣ�ˮƽ����Ϊx�������򣬴�ֱ����Ϊy��������

		Point2f& p = pictureInfo.keyPoints[i].pt;
		if (p.x <= img.cols && p.y <= img.rows)
			pictureInfo.pointsColors.push_back(img.at<Vec3b>(p.y, p.x));
	}

	return pictureInfo;
}

PictureInfo GetPictureInfo(Mat img,Mat moveFieldJudgeMatrix)
{
	PictureInfo pictureInfo;

	Ptr<Feature2D> surf = xfeatures2d::SURF::create();

	vector<Vec3b> pointsColors;


	//��ȡ������
	surf->detect(img, pictureInfo.keyPoints, moveFieldJudgeMatrix);
	//����������������
	surf->compute(img, pictureInfo.keyPoints, pictureInfo.descriptors);
	//��ȡ�����㴦��BGRֵ
	pictureInfo.pointsColors.clear();
	for (int i = 0; i < pictureInfo.keyPoints.size(); i++)
	{
		//��ȡBGRֵʱ�����꣬��Ϊopencv��ͼ������ϵ�ǣ�
		//ԭ�������Ͻǣ�ˮƽ����Ϊx�������򣬴�ֱ����Ϊy��������

		Point2f& p = pictureInfo.keyPoints[i].pt;
		if (p.x <= img.cols && p.y <= img.rows)
			pictureInfo.pointsColors.push_back(img.at<Vec3b>(p.y, p.x));
	}

	return pictureInfo;
}