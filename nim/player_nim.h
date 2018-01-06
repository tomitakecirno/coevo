#pragma once
/*-------------------------
ニムで必要になるクラス
-------------------------*/
#include <iostream>
#include <random>
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
	double eval;
	std::vector<double> stra;
	std::vector<double> Result;

	p_data() {
		Init();
		Init_stra();
	}

	void Init();
	void Init_stra();
	void cal_fitness();
	bool input_stra(std::string fname);
	bool output_stra(std::string fname);
};
void p_data::Init() {
	eval = 0;
	Result.clear();
}
void p_data::Init_stra() 
{
	std::random_device rd;
	std::mt19937 mt(rd());

	int len = 0;
	stra.resize(W_SIZE);

	std::uniform_real_distribution<> dist_w1(
		-std::sqrt(6. / ((INPUT + 1) + MIDDLE)),
		std::sqrt(6. / ((INPUT + 1) + MIDDLE))
	);
	std::uniform_real_distribution<> dist_w2(
		-std::sqrt(6. / ((MIDDLE + 1) + OUTPUT)),
		std::sqrt(6. / ((MIDDLE + 1) + OUTPUT))
	);
	for (int i = 0; i < INPUT*MIDDLE; i++, len++) {
		stra[len] = dist_w1(mt);
	}
	for (int i = 0; i < MIDDLE*OUTPUT; i++, len++) {
		stra[len] = dist_w2(mt);
	}
}
void p_data::cal_fitness()
{
	eval = std::accumulate(Result.begin(), Result.end(), 0.0);
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
	stra.resize(W_SIZE);
	while (fin >> tmp && count < W_SIZE) {
		stra[count] = tmp;
		count++;
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
	int stra_size = int(stra.size());
	if (stra_size == 0) {
		std::cout << "戦略が空です" << std::endl;
		return false;
	}
	//std::cout << fname << std::endl;
	//show_vec_1(stra);
	for (auto &pi : stra) {
		fout << pi << " ";
	}
	return true;
}
//ニムで使うプレイヤーのクラス
class playerNim : public p_data {
public:
	int nitch;
	int lose;
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

