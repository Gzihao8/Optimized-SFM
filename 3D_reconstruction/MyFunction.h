#pragma once
#include<opencv2/opencv.hpp>
#include"MyDataStructure.h"

#ifndef _MYFUNCTION_H
#define _MYFUNCTION_H

std::vector<std::string> SearchFiles(std::string path, std::string type);

cv::Mat GetMoveFieldJudgeMatrix(std::vector<cv::Mat>pictures);

PictureInfo GetPictureInfo(cv::Mat img);

PictureInfo GetPictureInfo(cv::Mat img, cv::Mat moveFieldJudgeMatrix);

std::vector<cv::DMatch> GetKeyPointsMatch(cv::Mat queryDescriptors,cv::Mat trainDescriptors);

std::vector<std::vector<cv::Point2f>> GetCorrespondPoints(std::vector<cv::KeyPoint> firstPictureKeyPoints,\
														  std::vector<cv::KeyPoint> nextPictureKeyPoints,\
														  std::vector<cv::DMatch> keyPointsMatch);

std::vector<std::vector<cv::Vec3b>> GetColorsOfCorrespondPoints(std::vector<cv::Vec3b> firstPointsColors, \
																std::vector<cv::Vec3b> nextPointsColors,\
																std::vector<cv::DMatch> keyPointsMatch);

bool GetTransform(cv::Mat K, std::vector<cv::Point2f> firstPicturePoints, std::vector<cv::Point2f> nextPicturePoints, \
	cv::Mat& R, cv::Mat& T, cv::Mat& mask);

void Maskout(std::vector<cv::Point2f>& p1, cv::Mat& mask);

void Maskout(std::vector<cv::Vec3b>& p1, cv::Mat& mask);

void Triangle(cv::Mat& K, cv::Mat& R1, cv::Mat& T1, cv::Mat& R2, cv::Mat& T2, std::vector<cv::Point2f>& p1, std::vector<cv::Point2f>& p2, std::vector<cv::Point3f>& structure);

void Save(std::string file_name, std::vector<cv::Point3f>& structure, std::vector<cv::Vec3b>& colors);

void Save(std::string file_name, std::vector<cv::Mat>& rotations, std::vector<cv::Mat>& motions, std::vector<cv::Point3f>& structure, std::vector<cv::Vec3b>& colors);
#endif