#pragma once
#include <vector>
#include <unordered_map>  
#include <map>
#include <algorithm>
#include <iostream>


class pass_top
{
public:
	pass_top();
	pass_top(std::string file, std::string file_o, int top, int v, int m);
	virtual ~pass_top();
	void du1();
	void du2();
	bool du3();
	const char* _get_line(const char* buf, int* len);
	int cmp(const std::pair<std::string, int>& x, const std::pair<std::string, int>& y);
	void sortMapByValue(std::unordered_map<std::string, int>& tMap, std::vector<std::pair<std::string, int> >& tVector);
	void Go();
private:

public:
	std::string file = "";
	std::string file_o = "";
	int top = 1000;
	int v = 0;
	int m = 0;
};

