#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

bool GetTransform(Mat K, vector<Point2f> firstPicturePoints, vector<Point2f> nextPicturePoints, \
				  Mat& R, Mat& T, Mat& mask)
{
	// 根据内参数矩阵获取相机的焦距和光心坐标（主点坐标）
	double focal_length = 0.5 * (K.at<double>(0) + K.at<double>(4));
	Point2d principle_point(K.at<double>(2), K.at<double>(5));

	// 根据匹配点求取本征矩阵，使用RANSAC，进一步排除失配点
	Mat E = findEssentialMat(firstPicturePoints, nextPicturePoints, focal_length, principle_point, RANSAC, 0.999, 1.0, mask);
	if (E.empty())
	{
		return false;
	}

	double feasible_count = countNonZero(mask);	// 得到非零元素，即数组中的有效点
	// cout << (int)feasible_count << " - in - " << p1.size() << endl;

	// 对于RANSAC而言，outlier数量大于50%时，结果是不可靠的
	if (feasible_count <= 15 || (feasible_count / firstPicturePoints.size()) < 0.6)
	{
		return false;
	}

	// 分解本征矩阵，获取相对变换
	int pass_count = recoverPose(E, firstPicturePoints, nextPicturePoints, R, T, focal_length, principle_point, mask);

	// cout << "pass_count = " << pass_count << endl;

	// 同时位于两个相机前方的点的数量要足够大
	if (((double)pass_count) / feasible_count < 0.7)
	{
		return false;
	}
	return true;
}