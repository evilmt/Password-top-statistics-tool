// 密码top统计.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "pass_top.h"


using namespace std;

int main(int argc, char * argv[])
{
	string file =""; //处理文件
	string file_o = ""; //输出文件
	int top = 1000; //top
	int v=0; //详情 0 不显示 1 显示
	int m=1; //方法 1 逐行 2 整个文件一次性读完 3 内存映射
	cout << "欢迎使用密码Top统计工具Bate 1.0 by agsec MT哥" << endl;
	cout << "说明：命令行执行：ag_toppass.exe -u 需要处理的文件名 -o 输出的文件名 -t 需要多少排名（TOP）-v 是否显示详细信息 -m 方法默认逐行" << endl;
	cout << "例如：ag_toppass.exe -u pass.txt -o top1000.txt -t 1000" << endl;
	cout << "-u 需要处理的文件名 (必须)" << endl;
	cout << "-o 输出的文件名 (必须)" << endl;
	cout << "-t 需要多少排名（TOP）默认：1000" << endl;
	cout << "-v 是否显示详细信息 默认 否" << endl;
	cout << "-m 方法 1是逐行 2是一次性读完 1省内存速度慢 2需要内存较大 速度快 3 内存映射方法 针对大文件" << endl;
	
	int i = 1;
	while (i < argc)
	{
		string test = "-u";
		if (argv[i] == test || i >= argc)
		{
			file = argv[i + 1];
		}
		test = "-o";
		if (argv[i] == test || i >= argc)
		{
			file_o = argv[i + 1];
		}
		test = "-t";
		if (argv[i] == test || i >= argc)
		{
			top = atoi(argv[i + 1]);
		}
		test = "-v";
		if (argv[i] == test || i >= argc)
		{
			v = 1;
		}
		test = "-m";
		if (argv[i] == test || i >= argc)
		{
			m = atoi(argv[i + 1]);
		}
		//cout << i << "|" << argv[i] <<"|" << endl;
		i++;
	}

	if (file == "" || file_o == "")
	{
		cout << "请输入文件名." << endl;
		return 0;
	}

	/*
	cout << "---------------------------" << endl;
	cout << "file:" << file << endl;
	cout << "file_o:" << file_o << endl;
	cout << "top:" << top << endl;
	cout << "v:" << v << endl;
	cout << "m:" << m << endl;
	*/
	pass_top *agsec_passtop = new pass_top(file, file_o, top, v, m);
	agsec_passtop->Go();

	return 0;
	                                                 
}


