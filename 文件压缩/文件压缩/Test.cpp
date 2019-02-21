#include"FileCompress.h"
#include<iostream>
#include<Windows.h>
int main()
{

	std::cout << "<<<<<<<<<<<<<压缩启动<<<<<<<<<<<<<<" << std::endl;
	FileCompress fc;
	fc.CompressFile("1.png");
	std::cout << "<<<<<<<<<<<<<开始解压<<<<<<<<<<<<<<" << std::endl;
	fc.UnCompressFile("1.hzp");
	system("pause");
	return 0;

}