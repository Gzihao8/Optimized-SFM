#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

vector<DMatch> GetKeyPointsMatch(Mat queryDescriptors, Mat trainDescriptors)
{
	vector<DMatch> keyPointsMatch;

	vector<vector<DMatch>> knn_matches;
	BFMatcher matcher(NORM_L2);
	matcher.knnMatch(queryDescriptors, trainDescriptors, knn_matches, 2);

	// ��ȡ����Ratio Test����Сƥ��ľ���
	float min_dist = FLT_MAX;
	for (int r = 0; r < knn_matches.size(); ++r)
	{
		// Rotio Test
		if (knn_matches[r][0].distance > 0.6 * knn_matches[r][1].distance)
		{
			continue;
		}

		float dist = knn_matches[r][0].distance;
		if (dist < min_dist)
		{
			min_dist = dist;
		}
	}

	keyPointsMatch.clear();
	for (size_t r = 0; r < knn_matches.size(); ++r)
	{
		// �ų�������Ratio Test�ĵ��ƥ��������ĵ�
		if (
			knn_matches[r][0].distance > 0.6 * knn_matches[r][1].distance ||
			knn_matches[r][0].distance > 5 * max(min_dist, 10.0f)
			)
		{
			continue;
		}
		// ����ƥ���
		keyPointsMatch.push_back(knn_matches[r][0]);
	}

	return keyPointsMatch;
}
