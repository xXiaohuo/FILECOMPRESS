#pragma once  
#include <string>
#include <vector>
#include"huffman.hpp"
using namespace std;
typedef unsigned char UCH;
//构建权值和对应字符的关系
struct CharInfo
{
	UCH _ch;//字符
	unsigned long long _count;//出现的次数
	string _strCode;//字符对应的编码

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
