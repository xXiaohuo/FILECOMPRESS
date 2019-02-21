#include "FileCompress.h"
#include <iostream>
#include <vector>
#include <assert.h>
#include<stdlib.h>
#include<windows.h>
// #include "huffman.hpp"
#pragma warning (disable: 4996)
using namespace std;

FileCompress::FileCompress()
{
	_fileInfo.resize(256);
	//对字符和次数进行初始化
	 
	for (size_t i = 0; i < 256; ++i)
	{
		_fileInfo[i]._ch = i;
		_fileInfo[i]._count = 0;
	}

}

//获取huffman树的节点的编码
void FileCompress::GetHuffmanCode(HTNode<CharInfo> * pRoot)
{
	if (nullptr == pRoot)
	{
		return;
	}

	GetHuffmanCode(pRoot->_pLeft);
	GetHuffmanCode(pRoot->_pRight);
	//找到叶子节点，并判断叶子节点是他双亲节点的左还是右获取相应的编码
	if (nullptr == pRoot->_pLeft && nullptr == pRoot->_pRight)
	{
		HTNode<CharInfo>* pCur = pRoot;
		HTNode<CharInfo>*  pParent = pCur->_pParent;
	    //初始化编码，根据叶子节点中权值 寻找对应字符作为下标，然后给他的编码赋值
		//取到的编码是个反的，所以需要反转
		string & strCode = _fileInfo[pRoot->_weight._ch]._strCode;
		while (pParent)
		{

			if (pCur == pParent->_pLeft)
			{
				strCode += '0';

			}
			else{
				strCode += '1';
			}

			pCur = pParent;
			pParent = pCur->_pParent;
		}
		reverse(strCode.begin(), strCode.end());
		
	}
	
}

//写入头部信息
void FileCompress::WriteHeadInfo(FILE*pf, const std::string & strFileName)
{
	//1.源文件后缀
	string posFix = strFileName.substr(strFileName.rfind('.'));

	//2.有效编码的行数
	//3.有效字符以及出现次数
	string strCharCount;
	//有效编码的行数
	size_t LineCount = 0;
	char szCount[32] = { 0 };
	for (size_t i = 0; i < 256; ++i)
	{
		if (0 != _fileInfo[i]._count)
		{
			strCharCount += _fileInfo[i]._ch;
			strCharCount += ',';
			memset(szCount, 0, 32);
			//转换为字符
			_itoa(_fileInfo[i]._count, szCount, 10);
			strCharCount += szCount;
			strCharCount += "\n";
			LineCount++;
		}
	}

	string strHeadInfo;
	strHeadInfo += posFix;
	strHeadInfo += "\n";
	memset(szCount, 0, 32);
	_itoa(LineCount, szCount, 10);
	strHeadInfo += szCount;
	strHeadInfo += "\n";

	strHeadInfo += strCharCount;
	fwrite(strHeadInfo.c_str(), 1, strHeadInfo.size(), pf);

}








void FileCompress::CompressFile(const std::string strFilePath)
{
	//1.统计源文件中每个字符的出现次数
	FILE* fIn = fopen(strFilePath.c_str(), "rb");
	if (nullptr == fIn)
	{
		cout << "打开文件失败" << endl;
		return;
	}


	//2.通过数组来进行统计
	UCH *pReadBuff = new UCH[1024];
	while (true)
	{
		//循环读取，当读到没有数据的时候跳出循环
		//读到数据，放到对应的下标计数++
		size_t rdSize = fread(pReadBuff, 1, 1024, fIn);
		if (rdSize == 0)
		{
			break;
		}
		for (size_t i = 0; i < rdSize; ++i)
		{
			_fileInfo[pReadBuff[i]]._count++;
		
		}
		cout << "------正在解析------" << endl;
	}


	//3.以每个字符出现的次数作为权值创建 huffman树
	HuffmanTree<CharInfo> ht;
	ht.CreatHuffmanTree(_fileInfo, CharInfo(0));
	cout << "------正在压缩10%------" << endl;

	//4.通过HUFFMAN树获取每个字符的编码
	GetHuffmanCode(ht.GetRoot());
	cout << "------正在压缩20%------" << endl;

	//5.用获取到的huffman编码来重新改写文件
	//新文件
	FILE *Fout = fopen("1.hzp", "wb");
	UCH ch = 0;
	UCH bitCount = 0;
	assert(Fout);

	//6.改写前将头部协议信息发送过去
	WriteHeadInfo(Fout,strFilePath);
	cout << "------正在压缩30%------" << endl;
	//7.改写文件并发送
	//设置读取文件的位置从开头开始重新读取
	//因为在上次读取的时候已经读取过lfin，所以读取的位置发生了改变所以需要重新设置读取位置
	fseek(fIn, 0, SEEK_SET);
	while (true)
	{
		//读取到buff中
		size_t rdSize = fread(pReadBuff, 1, 1024, fIn);
		if (rdSize == 0)
		{
			break;
		}		
		//用编码改写文件
		for (size_t i=0; i < rdSize; ++i)
		{
			//获取对应字符编码
			string &strCode = _fileInfo[pReadBuff[i]]._strCode;
			//strCode="111";
			//根据对应的字符编码改写
			//将每个字符的编码放入ch的比特位中
			//ch=0000 0000;
			for (size_t j = 0; j < strCode.size(); ++j)
			{
				//编码为1的或上对应的最后一位
				//因为原本就有0所以不用处理0
				//每次进来之后左移一位
				    ch <<= 1;
				if (strCode[j] == '1')
					ch |= 1;
				
				//记录放入的次数，判断是否比特位满了
				//这个次数在放入0和1都会加
			    //满了将它发送出去

					bitCount++;
			  if (bitCount == 8)
			  {
						//将字符写入压缩文件中去
						//并将字符和计数清0
						fputc(ch, Fout);
						ch = 0;
						bitCount = 0;
			 }
		 
		   }
			
		}
	}
	
	//出了循环可能有比特位没有放满，所以要对它进行处理
	if (bitCount > 0 && bitCount < 8)
	{
		ch <<= (8 - bitCount);//
		fputc(ch, Fout);
	}
	delete[] pReadBuff;
	fclose (Fout);
	fclose(fIn);

	for (size_t i = 4; i <=10; i++)
	{
		printf("------正在压缩%d%%------\n", i * 10);
		Sleep(100);
	}


	std::cout << "<<<<<<<<<<<<<压缩完成<<<<<<<<<<<<<<" << std::endl;
   }
 

 


  void FileCompress::GetLine(FILE*pf, string &strCotent)
   {

	   while (!feof(pf))
	   {
		   char ch = fgetc(pf);
		   if ('\n' == ch)
			   return;

		   strCotent += ch;
	   }
   }



 //文件的解压缩

 void FileCompress::UnCompressFile(const std::string strFilePath)
 {
	 string postFix = strFilePath.substr(strFilePath.rfind('.')+1);


	 if (postFix != "hzp")
	 {
		 cout << "打开文件格式不对" << endl;
		 return;
	 }

	 FILE* fIn = fopen(strFilePath.c_str(), "rb");

	 if (fIn == nullptr)
	 {
		 cout << "压缩文件打开失败" << endl;
		 return;
	 }

	 //从压缩文件中获取源文件后缀
	 postFix = "";
	 GetLine(fIn, postFix);

	 //从压缩文件中获取字符编码信息
	 string strContent;
	 GetLine(fIn, strContent);
	 size_t lineCount = atoi(strContent.c_str());
	 size_t charCount = 0;
	 for (size_t i = 0; i < lineCount; ++i)
	 {
		 strContent = "";
		 GetLine(fIn, strContent);

		 //对\n的处理 读取到\n再读取一行
		 if (strContent == "")
		 {
			 strContent += '\n';
	        GetLine(fIn, strContent);
		 }
		 //将对应的编码信息放回原来的数组中
		 charCount=atoi(strContent.c_str() + 2);
		 //把数组的下标也改成无符号的要不然会发生越界
		 _fileInfo[(UCH)strContent[0]]._count = charCount;
		
	 }
	 cout << "------正在解析------" << endl;

	 //还原HUFFAMAN树
	 HuffmanTree<CharInfo> ht;
	 cout << "------正在解压10%------" << endl;
	 ht.CreatHuffmanTree(_fileInfo, CharInfo(0));
	 cout << "------正在解压20%------" << endl;
	 //解压缩
	 string strUNFileName("2");
	 strUNFileName += postFix;
	 FILE* FOut = fopen(strUNFileName.c_str(), "wb");
	 UCH * pReadBuff = new UCH[1024];
	 int pos = 7;
	 HTNode<CharInfo>*pCur = ht.GetRoot();
	 //保存文件的大小，即所有字符出现的次数
	 long long fileSize = pCur->_weight._count;
	 while (true)
	 {
		 size_t rdSize = fread(pReadBuff, 1, 1024, fIn);
		 if (0 == rdSize)
			 break;

		 for (size_t i = 0; i < rdSize; ++i)
		 {
			 //解压缩当前字节的压缩数据
			 //0X96 --->1001 0110
			 pos = 7;
			 for (size_t j = 0; j < 8;++j)
			 {
				 if (pReadBuff[i] & (1 << pos))
					 pCur = pCur->_pRight;
				 else
					 pCur = pCur->_pLeft;

				 if (nullptr == pCur->_pLeft && nullptr == pCur->_pRight)
				 {
					 fputc(pCur->_weight._ch, FOut);
					 pCur = ht.GetRoot();
					 --fileSize;
					 if (0 == fileSize)
						 break;
				 }

				 pos--;//移动的位数
				 //当读完一个字节跳出循环
				 if (pos < 0)
				 {
					 break;
				 }
			 }
		 }
	 }
	 delete[] pReadBuff;
	 fclose(fIn);
	 fclose(FOut);
	 for (size_t i = 3; i <= 10; i++)
	 {
		 printf("------正在解压%d%%------\n", i * 10);
		 Sleep(100);
	 }


	 std::cout << "<<<<<<<<<<<<<解压完成<<<<<<<<<<<<<<" << std::endl;
 }
