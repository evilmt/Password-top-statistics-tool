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
		cout << "请输入文件名." << endl;
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

void pass_top::du1() //c语言逐行读取
{
	clock_t startTime, endTime;
	startTime = clock();
	unordered_map<string, int> myMap;
	FILE * fp;
	char StrLine[1024]; //行 最大字符数量

	fp = fopen(this->file.c_str(), "rb");
	if (fp == NULL)
	{
		fputs("打开文件失败", stderr);
		exit(1);
	}
	while (!feof(fp))
	{
		fgets(StrLine, sizeof(StrLine), fp);  //读取一行
		int len = strlen(StrLine);
		if (StrLine[len - 2] == '\r') //去掉 回车 和换行
		{
			StrLine[len - 2] = '\0';
		}
		if (StrLine[len - 1] == '\n')
		{
			StrLine[len - 1] = '\0';
		}
		unordered_map<string, int>::iterator iter = myMap.find(StrLine);
		if (iter != myMap.end()) {  //重复值+1

			iter->second++;
		}
		else {  //第一次出现
			if (StrLine != "")
			{
				myMap.insert(pair<string, int>(StrLine, 1));
			}

		}
	}
	vector<pair<string, int>> tVector;
	sortMapByValue(myMap, tVector);//排序

	myMap.clear();
	unordered_map<string, int>().swap(myMap);

	FILE *fp1 = fopen(this->file_o.c_str(), "w+b");
	if (fp1 == NULL)
	{
		fputs("写入文件创建失败", stderr);
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
	tVector.clear(); //释放向量
	tVector.shrink_to_fit();
	endTime = clock();//计时结束
	cout << "AG安全团队密码TOP工具工具提示用时：" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "秒" << endl;
}

void pass_top::du2() //一次全读进内存
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
		fputs("打开文件失败", stderr);
		exit(1);
	}
	fseek(pFile, 0, SEEK_END);//获取文件大小
	lSize = ftell(pFile);
	rewind(pFile);
	buffer = new char[sizeof(char)*lSize + 10];// 申请文件大小的内存
	if (buffer == NULL)
	{
		fputs("分配内存失败", stderr);
		exit(2);
	}

	//读入整个文件到内存
	result = fread(buffer, 1, lSize, pFile);
	if (result != lSize)
	{
		fputs("程序申请的内存与文件大小不符", stderr);
		exit(3);
	}
	fclose(pFile);
	long long i = 0; //下标
	while (i < lSize)
	{
		string pass;
		while (buffer[i] != '\r' && buffer[i] != '\n' && i < lSize) //判断换行符和回车
		{
			pass += buffer[i];
			if (i < lSize)
			{
				i++;
			}
		}
		unordered_map<string, int>::iterator iter = myMap.find(pass);
		if (iter != myMap.end()) {  //重复值+1

			iter->second += 1;
		}
		else {  //第一次出现
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

	delete buffer; //释放
	if (buffer != nullptr)
	{
		buffer = nullptr;
	}
	vector<pair<string, int>> tVector;
	sortMapByValue(myMap, tVector);//排序

	myMap.clear();
	unordered_map<string, int>().swap(myMap);

	FILE *fp1 = fopen(this->file_o.c_str(), "w+b");
	if (fp1 == NULL)
	{
		fputs("写入文件创建失败", stderr);
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
	tVector.clear(); //释放向量
	tVector.shrink_to_fit();
	endTime = clock();//计时结束
	cout << "AG安全团队密码TOP工具工具提示用时：" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "秒" << endl;
}

bool pass_top::du3()  //内存映射 针对大文件
{
	unordered_map<string, int> myMap;
	clock_t startTime, endTime;
	startTime = clock();
	char * buffer;
	DWORD dAccessType = (GENERIC_READ | GENERIC_WRITE);
	DWORD dShareType = FILE_SHARE_READ | FILE_SHARE_WRITE;
	HANDLE hFile = CreateFile(this->file.c_str()
		, dAccessType				// 访问模式
		, dShareType				// 共享模式
		, NULL
		, OPEN_EXISTING				// 如果存在则打开，否则失败
		, FILE_FLAG_OVERLAPPED		// 。。。
		, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("文件不存在");
		system("pause");
		return 1;
	}
	cout <<"返回代码：" <<GetLastError()<<endl;
	DWORD dFileSize = GetFileSize(hFile, NULL);
	HANDLE hFileMap = CreateFileMapping(hFile
		, NULL
		, PAGE_READWRITE		// 注意，和createfile要一致
		, 0
		, dFileSize
		, NULL);

	if (hFileMap == NULL)
	{
		printf("创建映射句柄失败\n");
		CloseHandle(hFile);
		system("pause");
		return 1;
	}

	CloseHandle(hFile);


	// 每次取500M 这个最好不要太大500M还是过大，这儿为了说明问题，当让如果是只读的话也没有什么问题
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


		// 取映射句柄的某一部分，并映射到内存中，注意这是虚拟内存，实际内存中并没有文件，也就是说不管多大文件，内存占用很小
		// 当然io（磁盘使用量可能会很大）
		char* pFileBlock = (char*)MapViewOfFile(hFileMap
			, FILE_MAP_ALL_ACCESS		// 注意，和createfile要一致
			, 0
			, dBegin					// 开始位置
			, dLen);					// 映射长度


		if (pFileBlock == NULL)
		{
			printf("映射失败\n");
			CloseHandle(hFileMap);
			system("pause");
			return 1;
		}
		long long lSize = strlen(pFileBlock);

		buffer = new char[sizeof(char)*lSize + 10];
		memcpy(buffer, pFileBlock, strlen(pFileBlock));
		UnmapViewOfFile(pFileBlock);
		long long i = 0; //下标
		while (i < lSize)
		{
			string pass;
			while (buffer[i] != '\r' && buffer[i] != '\n' && i < lSize) //判断换行符和回车
			{
				pass += buffer[i];
				if (i < lSize)
				{
					i++;
				}
			}
			unordered_map<string, int>::iterator iter = myMap.find(pass);
			if (iter != myMap.end()) {  //重复值+1

				iter->second += 1;
			}
			else {  //第一次出现
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

		delete buffer; //释放
		if (buffer != nullptr)
		{
			buffer = nullptr;
		}
		// “释放”掉虚拟内存
		
		dBegin += dLen;
		vector<pair<string, int>> tVector;
		sortMapByValue(myMap, tVector);//排序

		myMap.clear();
		unordered_map<string, int>().swap(myMap);

		FILE *fp1 = fopen(this->file_o.c_str(), "w+b");
		if (fp1 == NULL)
		{
			fputs("写入文件创建失败", stderr);
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
		tVector.clear(); //释放向量
		tVector.shrink_to_fit();
		endTime = clock();//计时结束
		cout << "AG安全团队密码TOP工具工具提示用时：" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "秒" << endl;
	}


	CloseHandle(hFileMap);
	printf("\nSuccess!\n");
	return 0;
}