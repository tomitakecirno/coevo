#pragma once
/*-------------------------
ニムで必要になるクラス
-------------------------*/
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <cassert>
#include "config_nim.hpp"
#include "CsvModules_nim.h"
#include "../header/Usual_Methods.hpp"

class p_data {
public:
	int stra_len;
	int win;
	int eval;
	std::vector<int> stra;
	std::vector<double> Result;

	void Init();
	void Init_stra();
	void cal_fitness();
	bool input_stra(std::string fname);
	bool output_stra(std::string fname);
};
void p_data::Init() {
	win = 0;
	eval = 0;
	Result.clear();
	stra_len = (POLL1 + 1)*(POLL2 + 1)*(POLL3 + 1);
}
void p_data::Init_stra() 
{
	stra_len = (POLL1 + 1)*(POLL2 + 1)*(POLL3 + 1);
	stra.resize(stra_len);

	for (int i = 0; i < stra_len; i++) {
		stra[i] = GetRand_Int(2);
	}
}
void p_data::cal_fitness()
{
	int sum = std::accumulate(Result.begin(), Result.end(), 0);
	eval = 0;
	eval = sum;
}
bool p_data::input_stra(const std::string fname)
{
	std::ifstream fin(fname);
	if (fin.fail()) {
		std::cout << "入力ファイルをオープンできません :" << fname << std::endl;
		return false;
	}

	//std::cout << fname << std::endl;
	int count = 0;
	int tmp;
	while (fin >> tmp) {
		stra.push_back(tmp);
	}

	if (count + 1 == STRA_LEN) {
		std::cout << "size error" << std::endl;
		return false;
	}
	return true;
}
bool p_data::output_stra(const std::string fname)
{
	std::ofstream fout(fname);
	if (fout.fail()) {
		std::cout << "出力ファイルをオープンできません :" << fname << std::endl;
		return false;
	}
	if (stra_len == 0) {
		std::cout << "戦略が空です" << std::endl;
		return false;
	}
	for (int i = 0; i < stra_len; i++) {
		fout << stra[i] << " ";
	}
	return true;
}
//ニムで使うプレイヤーのクラス
class playerNim : public p_data {
public:
	int nitch;
	std::vector<int> List1;
	std::vector<int> List2;
	std::vector<int> List3;

	void Init_pn();
};
void playerNim::Init_pn() {
	Init();
	nitch = 0;
	List1.clear();
	List2.clear();
	List3.clear();
}

