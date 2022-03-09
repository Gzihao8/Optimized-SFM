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

	//提取特征点
	surf->detect(img,pictureInfo.keyPoints);
	//计算特征点描述符
	surf->compute(img,pictureInfo.keyPoints,pictureInfo.descriptors);
	//提取特征点处的BGR值
	pictureInfo.pointsColors.clear();
	for (int i = 0; i < pictureInfo.keyPoints.size();i++)
	{
		//提取BGR值时的坐标，因为opencv中图像坐标系是：
		//原点在左上角，水平向右为x轴正方向，垂直向下为y轴正方向

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


	//提取特征点
	surf->detect(img, pictureInfo.keyPoints, moveFieldJudgeMatrix);
	//计算特征点描述符
	surf->compute(img, pictureInfo.keyPoints, pictureInfo.descriptors);
	//提取特征点处的BGR值
	pictureInfo.pointsColors.clear();
	for (int i = 0; i < pictureInfo.keyPoints.size(); i++)
	{
		//提取BGR值时的坐标，因为opencv中图像坐标系是：
		//原点在左上角，水平向右为x轴正方向，垂直向下为y轴正方向

		Point2f& p = pictureInfo.keyPoints[i].pt;
		if (p.x <= img.cols && p.y <= img.rows)
			pictureInfo.pointsColors.push_back(img.at<Vec3b>(p.y, p.x));
	}

	return pictureInfo;
}