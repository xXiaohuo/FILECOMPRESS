#include"FileCompress.h"
#include<iostream>
#include<Windows.h>
int main()
{

	std::cout << "<<<<<<<<<<<<<ѹ������<<<<<<<<<<<<<<" << std::endl;
	FileCompress fc;
	fc.CompressFile("1.png");
	std::cout << "<<<<<<<<<<<<<��ʼ��ѹ<<<<<<<<<<<<<<" << std::endl;
	fc.UnCompressFile("1.hzp");
	system("pause");
	return 0;

}