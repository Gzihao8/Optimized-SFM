#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

vector<vector<Point2f>> GetCorrespondPoints(vector<KeyPoint> firstPictureKeyPoints,\
											vector<KeyPoint> nextPictureKeyPoints,\
											vector<DMatch> keyPointsMatch)
{
	vector<vector<Point2f>> correspondPoints;
	vector<Point2f> firstPicturePoint;
	vector<Point2f> nextPicturePoint;

	firstPicturePoint.clear();
	nextPicturePoint.clear();
	for (int i = 0; i < keyPointsMatch.size(); i++)
	{
		firstPicturePoint.push_back(firstPictureKeyPoints[keyPointsMatch[i].queryIdx].pt);
		nextPicturePoint.push_back(nextPictureKeyPoints[keyPointsMatch[i].trainIdx].pt);
	}

	correspondPoints.clear();
	correspondPoints.push_back(firstPicturePoint);
	correspondPoints.push_back(nextPicturePoint);

	return correspondPoints;
}