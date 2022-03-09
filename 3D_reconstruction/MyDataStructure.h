#pragma once
#include<opencv2/opencv.hpp>

#ifndef _MYDATASTRUCTURE_H
#define _MYDATASTRUCTURE_H
struct PictureInfo
{
	std::vector<cv::KeyPoint> keyPoints;
	cv::Mat descriptors;
	std::vector<cv::Vec3b> pointsColors;
};

#endif