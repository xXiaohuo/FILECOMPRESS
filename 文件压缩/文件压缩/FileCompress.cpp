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
	//���ַ��ʹ������г�ʼ��
	 
	for (size_t i = 0; i < 256; ++i)
	{
		_fileInfo[i]._ch = i;
		_fileInfo[i]._count = 0;
	}

}

//��ȡhuffman���Ľڵ�ı���
void FileCompress::GetHuffmanCode(HTNode<CharInfo> * pRoot)
{
	if (nullptr == pRoot)
	{
		return;
	}

	GetHuffmanCode(pRoot->_pLeft);
	GetHuffmanCode(pRoot->_pRight);
	//�ҵ�Ҷ�ӽڵ㣬���ж�Ҷ�ӽڵ�����˫�׽ڵ�������һ�ȡ��Ӧ�ı���
	if (nullptr == pRoot->_pLeft && nullptr == pRoot->_pRight)
	{
		HTNode<CharInfo>* pCur = pRoot;
		HTNode<CharInfo>*  pParent = pCur->_pParent;
	    //��ʼ�����룬����Ҷ�ӽڵ���Ȩֵ Ѱ�Ҷ�Ӧ�ַ���Ϊ�±꣬Ȼ������ı��븳ֵ
		//ȡ���ı����Ǹ����ģ�������Ҫ��ת
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

//д��ͷ����Ϣ
void FileCompress::WriteHeadInfo(FILE*pf, const std::string & strFileName)
{
	//1.Դ�ļ���׺
	string posFix = strFileName.substr(strFileName.rfind('.'));

	//2.��Ч���������
	//3.��Ч�ַ��Լ����ִ���
	string strCharCount;
	//��Ч���������
	size_t LineCount = 0;
	char szCount[32] = { 0 };
	for (size_t i = 0; i < 256; ++i)
	{
		if (0 != _fileInfo[i]._count)
		{
			strCharCount += _fileInfo[i]._ch;
			strCharCount += ',';
			memset(szCount, 0, 32);
			//ת��Ϊ�ַ�
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
	//1.ͳ��Դ�ļ���ÿ���ַ��ĳ��ִ���
	FILE* fIn = fopen(strFilePath.c_str(), "rb");
	if (nullptr == fIn)
	{
		cout << "���ļ�ʧ��" << endl;
		return;
	}


	//2.ͨ������������ͳ��
	UCH *pReadBuff = new UCH[1024];
	while (true)
	{
		//ѭ����ȡ��������û�����ݵ�ʱ������ѭ��
		//�������ݣ��ŵ���Ӧ���±����++
		size_t rdSize = fread(pReadBuff, 1, 1024, fIn);
		if (rdSize == 0)
		{
			break;
		}
		for (size_t i = 0; i < rdSize; ++i)
		{
			_fileInfo[pReadBuff[i]]._count++;
		
		}
		cout << "------���ڽ���------" << endl;
	}


	//3.��ÿ���ַ����ֵĴ�����ΪȨֵ���� huffman��
	HuffmanTree<CharInfo> ht;
	ht.CreatHuffmanTree(_fileInfo, CharInfo(0));
	cout << "------����ѹ��10%------" << endl;

	//4.ͨ��HUFFMAN����ȡÿ���ַ��ı���
	GetHuffmanCode(ht.GetRoot());
	cout << "------����ѹ��20%------" << endl;

	//5.�û�ȡ����huffman���������¸�д�ļ�
	//���ļ�
	FILE *Fout = fopen("1.hzp", "wb");
	UCH ch = 0;
	UCH bitCount = 0;
	assert(Fout);

	//6.��дǰ��ͷ��Э����Ϣ���͹�ȥ
	WriteHeadInfo(Fout,strFilePath);
	cout << "------����ѹ��30%------" << endl;
	//7.��д�ļ�������
	//���ö�ȡ�ļ���λ�ôӿ�ͷ��ʼ���¶�ȡ
	//��Ϊ���ϴζ�ȡ��ʱ���Ѿ���ȡ��lfin�����Զ�ȡ��λ�÷����˸ı�������Ҫ�������ö�ȡλ��
	fseek(fIn, 0, SEEK_SET);
	while (true)
	{
		//��ȡ��buff��
		size_t rdSize = fread(pReadBuff, 1, 1024, fIn);
		if (rdSize == 0)
		{
			break;
		}		
		//�ñ����д�ļ�
		for (size_t i=0; i < rdSize; ++i)
		{
			//��ȡ��Ӧ�ַ�����
			string &strCode = _fileInfo[pReadBuff[i]]._strCode;
			//strCode="111";
			//���ݶ�Ӧ���ַ������д
			//��ÿ���ַ��ı������ch�ı���λ��
			//ch=0000 0000;
			for (size_t j = 0; j < strCode.size(); ++j)
			{
				//����Ϊ1�Ļ��϶�Ӧ�����һλ
				//��Ϊԭ������0���Բ��ô���0
				//ÿ�ν���֮������һλ
				    ch <<= 1;
				if (strCode[j] == '1')
					ch |= 1;
				
				//��¼����Ĵ������ж��Ƿ����λ����
				//��������ڷ���0��1�����
			    //���˽������ͳ�ȥ

					bitCount++;
			  if (bitCount == 8)
			  {
						//���ַ�д��ѹ���ļ���ȥ
						//�����ַ��ͼ�����0
						fputc(ch, Fout);
						ch = 0;
						bitCount = 0;
			 }
		 
		   }
			
		}
	}
	
	//����ѭ�������б���λû�з���������Ҫ�������д���
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
		printf("------����ѹ��%d%%------\n", i * 10);
		Sleep(100);
	}


	std::cout << "<<<<<<<<<<<<<ѹ�����<<<<<<<<<<<<<<" << std::endl;
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



 //�ļ��Ľ�ѹ��

 void FileCompress::UnCompressFile(const std::string strFilePath)
 {
	 string postFix = strFilePath.substr(strFilePath.rfind('.')+1);


	 if (postFix != "hzp")
	 {
		 cout << "���ļ���ʽ����" << endl;
		 return;
	 }

	 FILE* fIn = fopen(strFilePath.c_str(), "rb");

	 if (fIn == nullptr)
	 {
		 cout << "ѹ���ļ���ʧ��" << endl;
		 return;
	 }

	 //��ѹ���ļ��л�ȡԴ�ļ���׺
	 postFix = "";
	 GetLine(fIn, postFix);

	 //��ѹ���ļ��л�ȡ�ַ�������Ϣ
	 string strContent;
	 GetLine(fIn, strContent);
	 size_t lineCount = atoi(strContent.c_str());
	 size_t charCount = 0;
	 for (size_t i = 0; i < lineCount; ++i)
	 {
		 strContent = "";
		 GetLine(fIn, strContent);

		 //��\n�Ĵ��� ��ȡ��\n�ٶ�ȡһ��
		 if (strContent == "")
		 {
			 strContent += '\n';
	        GetLine(fIn, strContent);
		 }
		 //����Ӧ�ı�����Ϣ�Ż�ԭ����������
		 charCount=atoi(strContent.c_str() + 2);
		 //��������±�Ҳ�ĳ��޷��ŵ�Ҫ��Ȼ�ᷢ��Խ��
		 _fileInfo[(UCH)strContent[0]]._count = charCount;
		
	 }
	 cout << "------���ڽ���------" << endl;

	 //��ԭHUFFAMAN��
	 HuffmanTree<CharInfo> ht;
	 cout << "------���ڽ�ѹ10%------" << endl;
	 ht.CreatHuffmanTree(_fileInfo, CharInfo(0));
	 cout << "------���ڽ�ѹ20%------" << endl;
	 //��ѹ��
	 string strUNFileName("2");
	 strUNFileName += postFix;
	 FILE* FOut = fopen(strUNFileName.c_str(), "wb");
	 UCH * pReadBuff = new UCH[1024];
	 int pos = 7;
	 HTNode<CharInfo>*pCur = ht.GetRoot();
	 //�����ļ��Ĵ�С���������ַ����ֵĴ���
	 long long fileSize = pCur->_weight._count;
	 while (true)
	 {
		 size_t rdSize = fread(pReadBuff, 1, 1024, fIn);
		 if (0 == rdSize)
			 break;

		 for (size_t i = 0; i < rdSize; ++i)
		 {
			 //��ѹ����ǰ�ֽڵ�ѹ������
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

				 pos--;//�ƶ���λ��
				 //������һ���ֽ�����ѭ��
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
		 printf("------���ڽ�ѹ%d%%------\n", i * 10);
		 Sleep(100);
	 }


	 std::cout << "<<<<<<<<<<<<<��ѹ���<<<<<<<<<<<<<<" << std::endl;
 }
