#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

vector<vector<Vec3b>> GetColorsOfCorrespondPoints(vector<Vec3b> firstPointsColors, \
												  vector<Vec3b> nextPointsColors, \
												  vector<DMatch> keyPointsMatch)
{
	vector<vector<Vec3b>> colorsOfCorrespondPoints;
	vector<Vec3b> firstPictureColors;
	vector<Vec3b> nextPictureColors;

	firstPictureColors.clear();
	nextPictureColors.clear();
	for (int i = 0; i < keyPointsMatch.size(); i++)
	{
		firstPictureColors.push_back(firstPointsColors[keyPointsMatch[i].queryIdx]);
		nextPictureColors.push_back(nextPointsColors[keyPointsMatch[i].trainIdx]);
	}

	colorsOfCorrespondPoints.clear();
	colorsOfCorrespondPoints.push_back(firstPictureColors);
	colorsOfCorrespondPoints.push_back(nextPictureColors);

	return colorsOfCorrespondPoints;
}