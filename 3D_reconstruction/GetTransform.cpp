#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

bool GetTransform(Mat K, vector<Point2f> firstPicturePoints, vector<Point2f> nextPicturePoints, \
				  Mat& R, Mat& T, Mat& mask)
{
	// �����ڲ��������ȡ����Ľ���͹������꣨�������꣩
	double focal_length = 0.5 * (K.at<double>(0) + K.at<double>(4));
	Point2d principle_point(K.at<double>(2), K.at<double>(5));

	// ����ƥ�����ȡ��������ʹ��RANSAC����һ���ų�ʧ���
	Mat E = findEssentialMat(firstPicturePoints, nextPicturePoints, focal_length, principle_point, RANSAC, 0.999, 1.0, mask);
	if (E.empty())
	{
		return false;
	}

	double feasible_count = countNonZero(mask);	// �õ�����Ԫ�أ��������е���Ч��
	// cout << (int)feasible_count << " - in - " << p1.size() << endl;

	// ����RANSAC���ԣ�outlier��������50%ʱ������ǲ��ɿ���
	if (feasible_count <= 15 || (feasible_count / firstPicturePoints.size()) < 0.6)
	{
		return false;
	}

	// �ֽⱾ�����󣬻�ȡ��Ա任
	int pass_count = recoverPose(E, firstPicturePoints, nextPicturePoints, R, T, focal_length, principle_point, mask);

	// cout << "pass_count = " << pass_count << endl;

	// ͬʱλ���������ǰ���ĵ������Ҫ�㹻��
	if (((double)pass_count) / feasible_count < 0.7)
	{
		return false;
	}
	return true;
}