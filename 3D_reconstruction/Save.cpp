#include<opencv2/opencv.hpp>
#include<fstream>

using namespace std;
using namespace cv;

void Save(string file_name,vector<Point3f>& structure, vector<Vec3b>& colors)
{
	int n = (int)structure.size();

	ofstream write;
	write.open("../results/structure.ply");
	write << "ply" << endl;
	write << "format ascii 1.0" << endl;
	write << "element vertex " << n << endl;
	write << "property float x" << endl;
	write << "property float y" << endl;
	write << "property float z" << endl;
	write << "property uchar red" << endl;
	write << "property uchar green" << endl;
	write << "property uchar blue" << endl;
	write << "end_header" << endl;
	for (int i = 0; i < n; i++)
	{
		write << structure[i].x << " " << structure[i].y << " " << structure[i].z << " " << (int)colors[i][2] << " " << (int)colors[i][1] << " " << (int)colors[i][0] << endl;
	}
	write.close();
}

void Save(string file_name, vector<Mat>& rotations, vector<Mat>& motions, vector<Point3f>& structure, vector<Vec3b>& colors)
{
	int n = (int)rotations.size();

	FileStorage fs(file_name, FileStorage::WRITE);
	fs << "Camera Count" << n;
	fs << "Point Count" << (int)structure.size();

	fs << "Rotations" << "[";
	for (size_t i = 0; i < n; ++i)
	{
		fs << rotations[i];
	}
	fs << "]";

	fs << "Motions" << "[";
	for (size_t i = 0; i < n; i++)
	{
		fs << motions[i];
	}
	fs << "]";

	fs << "Points" << "[";
	for (size_t i = 0; i < structure.size(); ++i)
	{
		fs << structure[i];
	}
	fs << "]";

	fs << "Colors" << "[";
	for (size_t i = 0; i < colors.size(); ++i)
	{
		fs << colors[i];
	}
	fs << "]";

	fs.release();

}