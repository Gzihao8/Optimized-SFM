#include<opencv2/opencv.hpp>
#include<io.h>
#include"MyFunction.h"

using namespace std;
using namespace cv;

void main()
{
	vector<string> files = SearchFiles("../imags", "jpg");

	vector<Mat> pictures;

	for (int i = 0; i < files.size(); i++)
	{
		pictures.push_back(imread(files[i]));
	}

	Mat moveFieldJudgeMatrix = GetMoveFieldJudgeMatrix(pictures);

	//applyColorMap(moveFieldJudgeMatrix, moveFieldJudgeMatrix, COLORMAP_JET);

	namedWindow("window", CV_WINDOW_NORMAL);
	imshow("window", moveFieldJudgeMatrix);
	waitKey(0);

	//计算时间
	clock_t start, finish;
	double duration;
	start = clock();

	//提取图片特征点信息
	vector<PictureInfo> allPicturesInfo;
	for (int i = 0; i < pictures.size(); i++)
	{
		//使用掩码
		allPicturesInfo.push_back(GetPictureInfo(pictures[i],moveFieldJudgeMatrix));

		//不使用掩码
		//allPicturesInfo.push_back(GetPictureInfo(pictures[i]));
	}

	//cout << allPicturesInfo[0].descriptors << endl;

	//进行特征点匹配
	vector<vector<DMatch>> allMatches;
	for (int i = 0; i < pictures.size() - 1; i++)
	{
		allMatches.push_back(GetKeyPointsMatch(allPicturesInfo[i].descriptors, allPicturesInfo[i + 1].descriptors));
	}

	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << duration << " seconds" << endl;

	//for (int i = 0; i < allMatches.size(); i++)
	//{
	//	Mat matchResult;
	//	drawMatches(pictures[i], allPicturesInfo[i].keyPoints, pictures[i+1], allPicturesInfo[i+1].keyPoints, allMatches[i], matchResult);
	//	//namedWindow("match", CV_WINDOW_NORMAL);
	//	//imshow("match", matchResult);
	//	//waitKey(0);
	//	string s = "../results/test" + to_string(i) + ".jpg";
	//	imwrite(s, matchResult);
	//}

	//得到图0和图1的匹配点对的像素坐标
	vector<vector<Point2f>> correspondPoints;
	correspondPoints = GetCorrespondPoints(allPicturesInfo[0].keyPoints, allPicturesInfo[1].keyPoints, allMatches[0]);
	//得到图0和图1的匹配点对的BGR值
	vector<vector<Vec3b>> colorsOfCorrespondPoints;
	colorsOfCorrespondPoints = GetColorsOfCorrespondPoints(allPicturesInfo[0].pointsColors, allPicturesInfo[1].pointsColors, allMatches[0]);

	Mat K(Matx33d(
		2759.48, 0, 1520.69,
		0, 2764.16, 1006.81,
		0, 0, 1));
	vector<Vec3b> colors;//储存颜色
	Mat R, T;	// 旋转矩阵和平移向量
	Mat mask;	// mask中大于零的点代表匹配点，等于零的点代表失配点
	//保存所有图像的旋转平移矩阵
	vector<Mat> rotations;
	vector<Mat> motions;
	//根据图0和图1的匹配点对的像素坐标，求解图1摄像机相对于图0摄像机的位姿R,T
	GetTransform(K, correspondPoints[0], correspondPoints[1], R, T, mask);
	Maskout(correspondPoints[0], mask);
	Maskout(correspondPoints[1], mask);
	Maskout(colorsOfCorrespondPoints[0], mask);

	colors = colorsOfCorrespondPoints[0];
	
	Mat R0 = Mat::eye(3, 3, CV_64FC1);
	Mat T0 = Mat::zeros(3, 1, CV_64FC1);
	//按顺序储存旋转平移矩阵
	rotations.push_back(R0);
	motions.push_back(T0);
	rotations.push_back(R);
	motions.push_back(T);
	vector<Point3f> structure;
	Triangle(K, R0, T0, R, T, correspondPoints[0],correspondPoints[1],structure);	// 三角化

	//保存特征点与重建出来的三维点的对应关系
	vector<vector<int>>keyPointsTo3DPoints;
	keyPointsTo3DPoints.clear();
	keyPointsTo3DPoints.resize(allPicturesInfo.size());
	for (int i = 0; i < allPicturesInfo.size(); i++)
	{
		keyPointsTo3DPoints[i].resize(allPicturesInfo[i].keyPoints.size(), -1);
	}
	//填写头两幅图像重建出来的三维点与头两幅图像的特征点之间的对应关系
	int idx = 0;
	for (int i = 0; i < allMatches[0].size(); i++)
	{
		if (mask.at<uchar>(i) == 0)
		{
			continue;
		}
		keyPointsTo3DPoints[0][allMatches[0][i].queryIdx] = idx;
		keyPointsTo3DPoints[1][allMatches[0][i].trainIdx] = idx;
		idx++;
	}

	//通过增量方式处理余下的匹配对，进行三维重建，与图0和图1重建出来的三维点集structure融合
	for (int match = 1; match < allMatches.size(); match++)
	{
		Mat R, T, r;
		//PnP准备工作
		//用于储存三维点
		vector<Point3f> points3D;
		//用于储存在下一副图像中，与已重建的三维点对应的像素点
		vector<Point2f> pointsImage;
		points3D.clear();
		pointsImage.clear();
		for (int i = 0; i < allMatches[match].size(); i++)
		{
			int query_idx = allMatches[match][i].queryIdx;
			int train_idx = allMatches[match][i].trainIdx;
			int struct_idx = keyPointsTo3DPoints[match][query_idx];
			if (struct_idx < 0)//表明这个特征点还没重建成三维点
			{
				continue;
			}
			points3D.push_back(structure[struct_idx]);
			pointsImage.push_back(allPicturesInfo[match + 1].keyPoints[train_idx].pt);
		}

		// 求解变换矩阵
		solvePnPRansac(points3D, pointsImage, K, noArray(), r, T);
		// 将旋转向量转换为旋转矩阵
		Rodrigues(r, R);
		//储存旋转平移矩阵
		rotations.push_back(R);
		motions.push_back(T);

		//得到图match和图match+1的匹配点对的像素坐标
		vector<vector<Point2f>> correspondPoints;
		correspondPoints = GetCorrespondPoints(allPicturesInfo[match].keyPoints, allPicturesInfo[match+1].keyPoints, allMatches[match]);
		//得到图match和图match+1的匹配点对的BGR值
		vector<vector<Vec3b>> colorsOfCorrespondPoints;
		colorsOfCorrespondPoints = GetColorsOfCorrespondPoints(allPicturesInfo[match].pointsColors, allPicturesInfo[match+1].pointsColors, allMatches[match]);
		// 根据之前求得的R, T进行三角化三维重建
		vector<Point3f> next_structure;
		Triangle(K, rotations[match], motions[match], R, T, correspondPoints[0], correspondPoints[1], next_structure);

		//融合重建结果
		for (int i = 0; i < allMatches[match].size(); i++)
		{
			int query_idx = allMatches[match][i].queryIdx;
			int train_idx = allMatches[match][i].trainIdx;
			int struct_idx = keyPointsTo3DPoints[match][query_idx];
			if (struct_idx != (-1))//若该处的特征点已被重建成三维点，则下一副图像中，与此特征点匹配的特征点对应的三维点应该是同一个点
			{
				keyPointsTo3DPoints[match + 1][train_idx] = struct_idx;
				continue;
			}
			//若该处的特征点未被重建成三维点
			structure.push_back(next_structure[i]);
			colors.push_back(colorsOfCorrespondPoints[0][i]);
			keyPointsTo3DPoints[match][query_idx] = keyPointsTo3DPoints[match + 1][train_idx] = structure.size() - 1;
		}
	}

	//以ply的形式输出
	Save("../results/structure.ply", structure, colors);
	//以yml的形式输出
	//Save("../results/structure.yml", rotations, motions, structure, colors);
}