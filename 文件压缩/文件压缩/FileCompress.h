#pragma once  
#include <string>
#include <vector>
#include"huffman.hpp"
using namespace std;
typedef unsigned char UCH;
//����Ȩֵ�Ͷ�Ӧ�ַ��Ĺ�ϵ
struct CharInfo
{
	UCH _ch;//�ַ�
	unsigned long long _count;//���ֵĴ���
	string _strCode;//�ַ���Ӧ�ı���

	CharInfo(unsigned long long count = 0)
	:_count(count)
	{}
	
	CharInfo  operator+(const CharInfo & info)
	{
		return CharInfo(_count + info._count);
	}

	bool operator >(const CharInfo & info)
	{
		return _count >info._count;
	}
	
	bool operator !=(const CharInfo & info)const
	{
		return _count != info._count;
	}
};

class FileCompress
{
public:
	FileCompress();
	void CompressFile(const std::string strFilePath);
    void UnCompressFile(const std::string strFilePath);

private:
	void GetHuffmanCode(HTNode<CharInfo> * pRoot);
	void WriteHeadInfo(FILE*pf,const std::string & strFileName);
	void GetLine(FILE*pf, std::string &strContent);
	vector<CharInfo> _fileInfo;
};
