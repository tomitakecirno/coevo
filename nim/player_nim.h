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
	void Init();
	void Init_stra();
	void cal_fitness();
	bool input_stra(std::string fname);
	bool output_stra(std::string fname);
protected:
	int stra_len;
	int win;
	int eval;
	std::vector<int> stra;
	std::vector<int> Result;
};
void p_data::Init() {
	win = 0;
	eval = 0;
	Result.clear();
}
void p_data::Init_stra() 
{
	stra_len = int(std::pow(2, POLL1 + POLL2 + POLL3));
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
		std::cout << "出力ファイルをオープンできません :" << fname << std::endl;
		return false;
	}

	stra_len = int(std::pow(2, POLL1 + POLL2 + POLL3));
	stra.resize(stra_len);
	std::cout << fname << std::endl;
	int i = 0;
	int tmp;
	while (fin >> tmp) {
		stra[i] = tmp;
		i++;
	}

	if (i + 1 == stra_len) {
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
	for (int i = 0; i<stra_len; i++) {
		fout << stra[i] << " ";
	}
	return true;
}

//ニムで使うプレイヤーのクラス
class playerNim : public p_data {
public:
	void Init_pn();	//戦略初期化
	void input_nitch(int n);
	int get_eval();
	int get_nitch();
	bool input_list(std::vector<int> &list, int n);
	bool input_result(std::vector<int> &re);
	bool get_list(std::vector<int> &list, int n);
	bool get_result(std::vector<int> &re);
	bool get_stra(std::vector<int> &s);
protected:
	//戦略
	int nitch;
	std::vector<int> List1;
	std::vector<int> List2;
	std::vector<int> List3;
};
void playerNim::Init_pn() {
	Init();
	nitch = 0;
	List1.clear();
	List2.clear();
	List3.clear();
}
void playerNim::input_nitch(int n) {
	nitch = n;
}
int playerNim::get_eval() {
	return eval;
}
int playerNim::get_nitch() {
	return nitch;
}
bool playerNim::input_list(std::vector<int> &list, int n) {

	switch (n) {
	case 1:
		List1 = list;
		break;
	case 2:
		List2 = list;
		break;
	case 3:
		List3 = list;
		break;
	default:
		std::cout << "nが大きすぎます : input_list" << std::endl;
		exit(EXIT_FAILURE);
		break;
	}
	return true;
}
bool playerNim::input_result(std::vector<int> &re) {
	if (re.empty()) {
		std::cout << "結果のベクターが空です[IN]" << std::endl;
		return false;
	}
	Result = re;
	return true;
}
bool playerNim::get_list(std::vector<int> &list, int n) {
	switch (n) {
	case 1:
		list = List1;
		break;
	case 2:
		list = List2;
		break;
	case 3:
		list = List3;
		break;
	default:
		std::cout << "nが大きすぎます : get_list" << std::endl;
		exit(EXIT_FAILURE);
		break;
	}
	return true;
}
bool playerNim::get_result(std::vector<int> &re) {
	if (Result.empty()) {
		std::cout << "結果のベクターが空です[OUT]" << std::endl;
		return false;
	}
	re = Result;
	return true;
}
bool playerNim::get_stra(std::vector<int> &s) {
	if (stra.empty()) {
		std::cout << "戦略が空です : get_stra" << std::endl;
		return false;
	}
	s = stra;
	return true;
}
