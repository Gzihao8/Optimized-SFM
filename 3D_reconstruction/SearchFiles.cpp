#include<opencv2/opencv.hpp>
#include<io.h>
#include<stdlib.h>

std::vector<std::string> SearchFiles(std::string path,std::string type)
{
	std::vector<std::string> files;

	std::string search = path + "/*." + type;
	const char *to_search = search.c_str();                 //�����ҵ��ļ���֧��ͨ���

	long long handle;                                      //���ڲ��ҵľ��
	struct _finddata_t fileinfo;                          //�ļ���Ϣ�Ľṹ��
	handle = _findfirst(to_search, &fileinfo);           //��һ�β���
	if (-1 == handle)
	{
		std::cout << "û���ҵ���Ӧ��ʽ��ͼƬ" << std::endl;
		abort();
	}
	files.clear();
	files.push_back(path +"/"+ fileinfo.name);                       //��ӡ���ҵ����ļ����ļ���
	while (!_findnext(handle, &fileinfo))               //ѭ�������������ϵ��ļ���֪���Ҳ���������Ϊֹ
	{
		files.push_back(path + "/" + fileinfo.name);
	}
	_findclose(handle);                                      //�����˹رվ��

	return files;
}