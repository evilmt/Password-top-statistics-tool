#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "pass_top.h"
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <time.h>
#include <cstringt.h>
#include <vector>
#include <windows.h>
#include <ctime>
#include <unordered_map>
#include <map>
#include <string>
#include <algorithm>
#include <fstream>
#include <thread>
#include <iterator>
#include <functional>

using namespace std;
using namespace std::placeholders;
pass_top::pass_top()
{



}

pass_top::pass_top(string file,string file_o, int top=1000, int v=0, int m=0)
{
	if (file == "" || file_o == "")         
	{
		cout << "�������ļ���." << endl;
		exit(1);
	}
	this->file = file;
	this->file_o = file_o;
	this->top = top;
	this->v = v;
	this->m = m;

}
void pass_top::Go()
{
	if (this->m == 1)
	{
		thread noob(&pass_top::du1, this);
		noob.join();
	}
	if (this->m == 2)
	{
		thread noob(&pass_top::du2, this);
		noob.join();
	}
	if (this->m == 3)
	{
		thread noob(&pass_top::du3, this);
		noob.join();
	}
}

pass_top::~pass_top()
{
}
const char* pass_top::_get_line(const char* buf, int* len)
{
	const char* tmp = buf;
	while (*tmp && (*tmp != 0x0d && *tmp != 0x0a && *tmp != '\n')) ++tmp;
	//while(*tmp && (*tmp != 0x0d || *tmp != 0x0a )) ++tmp;
	*len = tmp - buf; //

	if (*tmp == 0) return NULL;

	// skip New-Line char
	if (*tmp == 0x0d) { // Windows style New-Line 0x0d 0x0a
		tmp += 2;
		//assert(*tmp == 0x0a);
	}//else Unix style New-Line 0x0a
	else {
		++tmp;
	}

	return tmp;
}

int pass_top::cmp(const pair<string, int>& x, const pair<string, int>& y)
{
	return x.second > y.second;
}

void pass_top::sortMapByValue(unordered_map<string, int>& tMap, vector<pair<string, int> >& tVector)
{
	for (unordered_map<string, int>::iterator curr = tMap.begin(); curr != tMap.end(); curr++)
		tVector.push_back(make_pair(curr->first, curr->second));
	sort(tVector.begin(), tVector.end(), [](const pair<string, int>& x, const pair<string, int>& y) {return x.second > y.second;; });
}

void pass_top::du1() //c�������ж�ȡ
{
	clock_t startTime, endTime;
	startTime = clock();
	unordered_map<string, int> myMap;
	FILE * fp;
	char StrLine[1024]; //�� ����ַ�����

	fp = fopen(this->file.c_str(), "rb");
	if (fp == NULL)
	{
		fputs("���ļ�ʧ��", stderr);
		exit(1);
	}
	while (!feof(fp))
	{
		fgets(StrLine, sizeof(StrLine), fp);  //��ȡһ��
		int len = strlen(StrLine);
		if (StrLine[len - 2] == '\r') //ȥ�� �س� �ͻ���
		{
			StrLine[len - 2] = '\0';
		}
		if (StrLine[len - 1] == '\n')
		{
			StrLine[len - 1] = '\0';
		}
		unordered_map<string, int>::iterator iter = myMap.find(StrLine);
		if (iter != myMap.end()) {  //�ظ�ֵ+1

			iter->second++;
		}
		else {  //��һ�γ���
			if (StrLine != "")
			{
				myMap.insert(pair<string, int>(StrLine, 1));
			}

		}
	}
	vector<pair<string, int>> tVector;
	sortMapByValue(myMap, tVector);//����

	myMap.clear();
	unordered_map<string, int>().swap(myMap);

	FILE *fp1 = fopen(this->file_o.c_str(), "w+b");
	if (fp1 == NULL)
	{
		fputs("д���ļ�����ʧ��", stderr);
		exit(1);
	}
	for (int i = 0; i < this->top; i++)
	{
		fprintf(fp1, tVector[i].first.c_str());
		fprintf(fp1, "\n");
		if (this->v == 1)
		{
			cout << tVector[i].first << ":---:" << tVector[i].second << endl;
		}
	}
	fclose(fp1);
	tVector.clear(); //�ͷ�����
	tVector.shrink_to_fit();
	endTime = clock();//��ʱ����
	cout << "AG��ȫ�Ŷ�����TOP���߹�����ʾ��ʱ��" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "��" << endl;
}

void pass_top::du2() //һ��ȫ�����ڴ�
{
	clock_t startTime, endTime;
	startTime = clock();
	unordered_map<string, int> myMap;
	FILE * pFile;
	char * buffer;
	size_t result;
	long long lSize;

	pFile = fopen(this->file.c_str(), "rb");
	if (pFile == NULL)
	{
		fputs("���ļ�ʧ��", stderr);
		exit(1);
	}
	fseek(pFile, 0, SEEK_END);//��ȡ�ļ���С
	lSize = ftell(pFile);
	rewind(pFile);
	buffer = new char[sizeof(char)*lSize + 10];// �����ļ���С���ڴ�
	if (buffer == NULL)
	{
		fputs("�����ڴ�ʧ��", stderr);
		exit(2);
	}

	//���������ļ����ڴ�
	result = fread(buffer, 1, lSize, pFile);
	if (result != lSize)
	{
		fputs("����������ڴ����ļ���С����", stderr);
		exit(3);
	}
	fclose(pFile);
	long long i = 0; //�±�
	while (i < lSize)
	{
		string pass;
		while (buffer[i] != '\r' && buffer[i] != '\n' && i < lSize) //�жϻ��з��ͻس�
		{
			pass += buffer[i];
			if (i < lSize)
			{
				i++;
			}
		}
		unordered_map<string, int>::iterator iter = myMap.find(pass);
		if (iter != myMap.end()) {  //�ظ�ֵ+1

			iter->second += 1;
		}
		else {  //��һ�γ���
			if (pass != "")
			{
				myMap.insert(pair<string, int>(pass, 1));
			}

		}
		if (i < lSize)
		{
			i++;
		}
	}

	delete buffer; //�ͷ�
	if (buffer != nullptr)
	{
		buffer = nullptr;
	}
	vector<pair<string, int>> tVector;
	sortMapByValue(myMap, tVector);//����

	myMap.clear();
	unordered_map<string, int>().swap(myMap);

	FILE *fp1 = fopen(this->file_o.c_str(), "w+b");
	if (fp1 == NULL)
	{
		fputs("д���ļ�����ʧ��", stderr);
		exit(1);
	}
	for (int i = 0; i < this->top; i++)
	{
		fprintf(fp1, tVector[i].first.c_str());
		fprintf(fp1, "\n");
		if (this->v == 1)
		{
			cout << tVector[i].first << ":---:" << tVector[i].second << endl;
		}
	}
	fclose(fp1);
	tVector.clear(); //�ͷ�����
	tVector.shrink_to_fit();
	endTime = clock();//��ʱ����
	cout << "AG��ȫ�Ŷ�����TOP���߹�����ʾ��ʱ��" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "��" << endl;
}

bool pass_top::du3()  //�ڴ�ӳ�� ��Դ��ļ�
{
	unordered_map<string, int> myMap;
	clock_t startTime, endTime;
	startTime = clock();
	char * buffer;
	DWORD dAccessType = (GENERIC_READ | GENERIC_WRITE);
	DWORD dShareType = FILE_SHARE_READ | FILE_SHARE_WRITE;
	HANDLE hFile = CreateFile(this->file.c_str()
		, dAccessType				// ����ģʽ
		, dShareType				// ����ģʽ
		, NULL
		, OPEN_EXISTING				// ���������򿪣�����ʧ��
		, FILE_FLAG_OVERLAPPED		// ������
		, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("�ļ�������");
		system("pause");
		return 1;
	}
	cout <<"���ش��룺" <<GetLastError()<<endl;
	DWORD dFileSize = GetFileSize(hFile, NULL);
	HANDLE hFileMap = CreateFileMapping(hFile
		, NULL
		, PAGE_READWRITE		// ע�⣬��createfileҪһ��
		, 0
		, dFileSize
		, NULL);

	if (hFileMap == NULL)
	{
		printf("����ӳ����ʧ��\n");
		CloseHandle(hFile);
		system("pause");
		return 1;
	}

	CloseHandle(hFile);


	// ÿ��ȡ500M �����ò�Ҫ̫��500M���ǹ������Ϊ��˵�����⣬���������ֻ���Ļ�Ҳû��ʲô����
	DWORD dFileBlock = 1024 * 1024 * 512;


	DWORD dBegin = 0;
	DWORD dLen = 0;
	while (dBegin < dFileSize)
	{
		if (dBegin + dFileBlock > dFileSize)
		{
			dLen = dFileSize - dBegin;
		}
		else
		{
			dLen = dFileBlock;
		}


		// ȡӳ������ĳһ���֣���ӳ�䵽�ڴ��У�ע�����������ڴ棬ʵ���ڴ��в�û���ļ���Ҳ����˵���ܶ���ļ����ڴ�ռ�ú�С
		// ��Ȼio������ʹ�������ܻ�ܴ�
		char* pFileBlock = (char*)MapViewOfFile(hFileMap
			, FILE_MAP_ALL_ACCESS		// ע�⣬��createfileҪһ��
			, 0
			, dBegin					// ��ʼλ��
			, dLen);					// ӳ�䳤��


		if (pFileBlock == NULL)
		{
			printf("ӳ��ʧ��\n");
			CloseHandle(hFileMap);
			system("pause");
			return 1;
		}
		long long lSize = strlen(pFileBlock);

		buffer = new char[sizeof(char)*lSize + 10];
		memcpy(buffer, pFileBlock, strlen(pFileBlock));
		UnmapViewOfFile(pFileBlock);
		long long i = 0; //�±�
		while (i < lSize)
		{
			string pass;
			while (buffer[i] != '\r' && buffer[i] != '\n' && i < lSize) //�жϻ��з��ͻس�
			{
				pass += buffer[i];
				if (i < lSize)
				{
					i++;
				}
			}
			unordered_map<string, int>::iterator iter = myMap.find(pass);
			if (iter != myMap.end()) {  //�ظ�ֵ+1

				iter->second += 1;
			}
			else {  //��һ�γ���
				if (pass != "")
				{
					myMap.insert(pair<string, int>(pass, 1));
				}

			}
			if (i < lSize)
			{
				i++;
			}
		}

		delete buffer; //�ͷ�
		if (buffer != nullptr)
		{
			buffer = nullptr;
		}
		// ���ͷš��������ڴ�
		
		dBegin += dLen;
		vector<pair<string, int>> tVector;
		sortMapByValue(myMap, tVector);//����

		myMap.clear();
		unordered_map<string, int>().swap(myMap);

		FILE *fp1 = fopen(this->file_o.c_str(), "w+b");
		if (fp1 == NULL)
		{
			fputs("д���ļ�����ʧ��", stderr);
			exit(1);
		}
		for (int i = 0; i < this->top; i++)
		{
			fprintf(fp1, tVector[i].first.c_str());
			fprintf(fp1, "\n");
			if (this->v == 1)
			{
				cout << tVector[i].first << ":---:" << tVector[i].second << endl;
			}
		}
		fclose(fp1);
		tVector.clear(); //�ͷ�����
		tVector.shrink_to_fit();
		endTime = clock();//��ʱ����
		cout << "AG��ȫ�Ŷ�����TOP���߹�����ʾ��ʱ��" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "��" << endl;
	}


	CloseHandle(hFileMap);
	printf("\nSuccess!\n");
	return 0;
}