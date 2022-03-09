#include<opencv2/opencv.hpp>
#include<io.h>
#include<stdlib.h>

std::vector<std::string> SearchFiles(std::string path,std::string type)
{
	std::vector<std::string> files;

	std::string search = path + "/*." + type;
	const char *to_search = search.c_str();                 //欲查找的文件，支持通配符

	long long handle;                                      //用于查找的句柄
	struct _finddata_t fileinfo;                          //文件信息的结构体
	handle = _findfirst(to_search, &fileinfo);           //第一次查找
	if (-1 == handle)
	{
		std::cout << "没有找到相应格式的图片" << std::endl;
		abort();
	}
	files.clear();
	files.push_back(path +"/"+ fileinfo.name);                       //打印出找到的文件的文件名
	while (!_findnext(handle, &fileinfo))               //循环查找其他符合的文件，知道找不到其他的为止
	{
		files.push_back(path + "/" + fileinfo.name);
	}
	_findclose(handle);                                      //别忘了关闭句柄

	return files;
}