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
	std::vector<double> nim_evaluation_vec;

	void Init();
	void Init_stra();
	void cal_fitness();
	bool input_stra(std::string fname);
	bool output_stra(std::string fname);
	void Init_Result();
	void cal_fitness_alfa(int opp_size, const std::vector<double> &opp_eval);
};
void p_data::Init() {
	eval = 0;
	Result.clear();
}
void p_data::Init_Result() {
	Result.clear();
}
void p_data::Init_stra()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<> dist(-INIT_VALUE, INIT_VALUE);
	std::uniform_real_distribution<> dist_0(0, INIT_VALUE);
	std::uniform_real_distribution<> dist_rad(0, 360);

	int len = 0;
	stra.resize(W_SIZE);
	switch (GAME_NUM) {
	case 0: //numbers
		for (int i = 0; i < DEM; i++) {
			stra[i] = dist_0(mt);
		}
		break;
	case 1: //nim
		for (int i = 0; i < INPUT*MIDDLE; i++, len++) {
			stra[len] = dist(mt);
		}
		for (int i = 0; i < MIDDLE*OUTPUT; i++, len++) {
			stra[len] = dist(mt);
		}
		break;
	case 2: //numbers_kai
		for (int i = 0; i < kaiDEM-1; i++,len++) {
			stra[len] = dist(mt);
		}
		stra[len] = dist_rad(mt);
		break;
	default:
		break;
	}
}
void p_data::cal_fitness()
{
	eval = std::accumulate(Result.begin(), Result.end(), 0.0);
}
void p_data::cal_fitness_alfa(int opp_size, const std::vector<double> &opp_eval)
{
	double sum = 0;
	for (int i = 0; i < opp_size; i++) {
		sum += Result[i] + ALFA*opp_eval[i] * Result[i];
	}
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
	double tmp;
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
	void Init_0();
	void Init_pn();
};
void playerNim::Init_pn() {
	Init();
	nitch = -1;
	List1.clear();
	List2.clear();
	List3.clear();
}
void playerNim::Init_0() {
	Init_Result();
	nitch = -1;
	List1.clear();
	List2.clear();
	List3.clear();
}

