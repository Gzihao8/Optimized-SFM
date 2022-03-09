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

	//����ʱ��
	clock_t start, finish;
	double duration;
	start = clock();

	//��ȡͼƬ��������Ϣ
	vector<PictureInfo> allPicturesInfo;
	for (int i = 0; i < pictures.size(); i++)
	{
		//ʹ������
		allPicturesInfo.push_back(GetPictureInfo(pictures[i],moveFieldJudgeMatrix));

		//��ʹ������
		//allPicturesInfo.push_back(GetPictureInfo(pictures[i]));
	}

	//cout << allPicturesInfo[0].descriptors << endl;

	//����������ƥ��
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

	//�õ�ͼ0��ͼ1��ƥ���Ե���������
	vector<vector<Point2f>> correspondPoints;
	correspondPoints = GetCorrespondPoints(allPicturesInfo[0].keyPoints, allPicturesInfo[1].keyPoints, allMatches[0]);
	//�õ�ͼ0��ͼ1��ƥ���Ե�BGRֵ
	vector<vector<Vec3b>> colorsOfCorrespondPoints;
	colorsOfCorrespondPoints = GetColorsOfCorrespondPoints(allPicturesInfo[0].pointsColors, allPicturesInfo[1].pointsColors, allMatches[0]);

	Mat K(Matx33d(
		2759.48, 0, 1520.69,
		0, 2764.16, 1006.81,
		0, 0, 1));
	vector<Vec3b> colors;//������ɫ
	Mat R, T;	// ��ת�����ƽ������
	Mat mask;	// mask�д�����ĵ����ƥ��㣬������ĵ����ʧ���
	//��������ͼ�����תƽ�ƾ���
	vector<Mat> rotations;
	vector<Mat> motions;
	//����ͼ0��ͼ1��ƥ���Ե��������꣬���ͼ1����������ͼ0�������λ��R,T
	GetTransform(K, correspondPoints[0], correspondPoints[1], R, T, mask);
	Maskout(correspondPoints[0], mask);
	Maskout(correspondPoints[1], mask);
	Maskout(colorsOfCorrespondPoints[0], mask);

	colors = colorsOfCorrespondPoints[0];
	
	Mat R0 = Mat::eye(3, 3, CV_64FC1);
	Mat T0 = Mat::zeros(3, 1, CV_64FC1);
	//��˳�򴢴���תƽ�ƾ���
	rotations.push_back(R0);
	motions.push_back(T0);
	rotations.push_back(R);
	motions.push_back(T);
	vector<Point3f> structure;
	Triangle(K, R0, T0, R, T, correspondPoints[0],correspondPoints[1],structure);	// ���ǻ�

	//�������������ؽ���������ά��Ķ�Ӧ��ϵ
	vector<vector<int>>keyPointsTo3DPoints;
	keyPointsTo3DPoints.clear();
	keyPointsTo3DPoints.resize(allPicturesInfo.size());
	for (int i = 0; i < allPicturesInfo.size(); i++)
	{
		keyPointsTo3DPoints[i].resize(allPicturesInfo[i].keyPoints.size(), -1);
	}
	//��дͷ����ͼ���ؽ���������ά����ͷ����ͼ���������֮��Ķ�Ӧ��ϵ
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

	//ͨ��������ʽ�������µ�ƥ��ԣ�������ά�ؽ�����ͼ0��ͼ1�ؽ���������ά�㼯structure�ں�
	for (int match = 1; match < allMatches.size(); match++)
	{
		Mat R, T, r;
		//PnP׼������
		//���ڴ�����ά��
		vector<Point3f> points3D;
		//���ڴ�������һ��ͼ���У������ؽ�����ά���Ӧ�����ص�
		vector<Point2f> pointsImage;
		points3D.clear();
		pointsImage.clear();
		for (int i = 0; i < allMatches[match].size(); i++)
		{
			int query_idx = allMatches[match][i].queryIdx;
			int train_idx = allMatches[match][i].trainIdx;
			int struct_idx = keyPointsTo3DPoints[match][query_idx];
			if (struct_idx < 0)//������������㻹û�ؽ�����ά��
			{
				continue;
			}
			points3D.push_back(structure[struct_idx]);
			pointsImage.push_back(allPicturesInfo[match + 1].keyPoints[train_idx].pt);
		}

		// ���任����
		solvePnPRansac(points3D, pointsImage, K, noArray(), r, T);
		// ����ת����ת��Ϊ��ת����
		Rodrigues(r, R);
		//������תƽ�ƾ���
		rotations.push_back(R);
		motions.push_back(T);

		//�õ�ͼmatch��ͼmatch+1��ƥ���Ե���������
		vector<vector<Point2f>> correspondPoints;
		correspondPoints = GetCorrespondPoints(allPicturesInfo[match].keyPoints, allPicturesInfo[match+1].keyPoints, allMatches[match]);
		//�õ�ͼmatch��ͼmatch+1��ƥ���Ե�BGRֵ
		vector<vector<Vec3b>> colorsOfCorrespondPoints;
		colorsOfCorrespondPoints = GetColorsOfCorrespondPoints(allPicturesInfo[match].pointsColors, allPicturesInfo[match+1].pointsColors, allMatches[match]);
		// ����֮ǰ��õ�R, T�������ǻ���ά�ؽ�
		vector<Point3f> next_structure;
		Triangle(K, rotations[match], motions[match], R, T, correspondPoints[0], correspondPoints[1], next_structure);

		//�ں��ؽ����
		for (int i = 0; i < allMatches[match].size(); i++)
		{
			int query_idx = allMatches[match][i].queryIdx;
			int train_idx = allMatches[match][i].trainIdx;
			int struct_idx = keyPointsTo3DPoints[match][query_idx];
			if (struct_idx != (-1))//���ô����������ѱ��ؽ�����ά�㣬����һ��ͼ���У����������ƥ����������Ӧ����ά��Ӧ����ͬһ����
			{
				keyPointsTo3DPoints[match + 1][train_idx] = struct_idx;
				continue;
			}
			//���ô���������δ���ؽ�����ά��
			structure.push_back(next_structure[i]);
			colors.push_back(colorsOfCorrespondPoints[0][i]);
			keyPointsTo3DPoints[match][query_idx] = keyPointsTo3DPoints[match + 1][train_idx] = structure.size() - 1;
		}
	}

	//��ply����ʽ���
	Save("../results/structure.ply", structure, colors);
	//��yml����ʽ���
	//Save("../results/structure.yml", rotations, motions, structure, colors);
}