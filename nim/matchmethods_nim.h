#pragma once
#include "config_nim.hpp"
#include <iostream>
#include <fstream>
#include <time.h>
#include "nim.h"
#include "coansmodule_nim.hpp"
#include "CsvModules_nim.h"

class Match {
public:
	Match(int method_p, int method_o, int t, int g, int g_p, int Cru = 0) {
		gene = g;
		per = g_p;
		method_pop = method_p;
		method_opp = method_o;
		cru_k = Cru;
		trial_pop = t;
		loop = (g / g_p) + 1;

		//result = std::vector<std::vector<double>>(gene / per + 1, std::vector<double>(KOT, 0));
		Make_CSV_Directory(method_pop);
		eval = std::vector<std::vector<double>>(loop, std::vector<double>(KO, 0));
		result2 = std::vector<std::vector<double>>(loop, std::vector<double>(4, 0));
		result2 = std::vector<std::vector<double>>(loop, std::vector<double>(KO, 0));
	}
	void evaluation();
private:
	int gene;			//世代数
	int per;			//区切り
	int loop;	//ループ
	int method_pop;
	int method_opp;
	int	cru_k;
	int trial_pop;
	std::string Dir;
	std::vector<p_data> pop;
	std::vector<p_data> opp;
protected:
	std::vector<std::vector<double>> result;
	std::vector<std::vector<double>> result2;
	std::vector<std::vector<double>> result3;
	std::vector<std::vector<double>> eval;
	void output_re_csv(int Opp_trial);
	bool output_ni_csv();
};

void Match::evaluation() 
{
	nim nim;
	p_data pop;
	char fname[50];
	std::vector<int> folder(TRIAL);

	for (int t = 0; t < TRIAL; t++) {
		sprintf_s(fname, "./nim/%d/%d", method_pop, t);
		folder[t] = count_folder(fname);
	}
	auto max = max_element(folder.begin(), folder.end());
	std::vector<std::vector<double>> result_total;
	result_total = std::vector<std::vector<double>>(*max, std::vector<double>(TRIAL, 0));

	for (int t = 0; t < TRIAL; t++) {
		const double start = clock();
		for (int f = 0; f < folder[t]; f++) {
			std::cout << t << ":" << f << ":" << " ..." << std::endl;
			sprintf_s(fname, "./nim/%d/%d/%d/best.dat", method_pop, t, f);

			if (!pop.input_stra(fname)) {
				exit(EXIT_FAILURE);
			}
			//std::cout << "pop_stra:";
			//show_vec_1(pop.stra);
			//nim.input_stra_first(pop.stra);
			result_total[f][t] = nim.nim_evaluation(pop.stra)*100;
			std::cout << "result :" << result_total[f][t] << std::endl;
		}
		const double end = clock();
		const double time = (end - start) / CLOCKS_PER_SEC;
	}
	sprintf_s(fname, "./csv/%d/currect_%d_%d.csv", method_pop, method_pop, BATTLE_PER);
	CsvModules::csv_fwrite(fname, result_total, BATTLE_PER);
}
bool Match::output_ni_csv() 
{
	std::cout << "output_ni_csv..." << std::endl;
	std::vector<std::vector<int>> pop_nitch;
	char fname[50];
	if (cru_k == 0) {
		sprintf_s(fname, "./csv/%d/Cruster_%d_%d_%d.csv", method_pop, method_pop, trial_pop, gene);
	}if (0 < cru_k) {
		sprintf_s(fname, "./csv/%d/Cruster_%d_%d_%d_%d.csv", method_pop, method_pop, trial_pop, gene, cru_k);
	}
	if (!CsvModules::GetContents(fname, pop_nitch)) {
		std::cout << std::endl;
		std::cout << "input error : " << fname << std::endl;
		return false;
	}
	/*
	for (auto &p : pop_nitch) {
		std::cout << "[";
		for (auto &q : p) {
			std::cout << q << ",";
		}
		std::cout << "]" << std::endl;
	}
	*/
	//クラスタ毎に対戦結果を出力
	for (int g = 0; g < gene / per + 1; g++) {
		int max = *max_element(pop_nitch[g].begin(), pop_nitch[g].end());
		std::vector<int> index(max+1, 0);
		std::vector<std::vector<double>> pop_dis(max+1);
		std::vector<std::vector<double>> pop_res(max+1);

		for (int i = 0; i < max+1; i++) {
			int count = int(std::count(pop_nitch[g].begin(), pop_nitch[g].end(), i));
			pop_dis[i].resize(count);
		}

		//戦略間距離
		std::vector<double> stra_0;
		p_data pop;
		int stra_len = (POLL1 + 1)*(POLL2 + 1)*(POLL3 + 1);
		stra_0.assign(stra_len, 0);

		for (int i = 0; i < KO; i++) {
			char fname[50];
			int tmp_nitch = pop_nitch[g][i];
			int tmp_index = index[tmp_nitch];

			//戦略格納
			sprintf_s(fname, "./nim/%d/%d/%d/%d.dat", method_pop, trial_pop, gene, i);
			pop.input_stra(fname);

			pop_dis[tmp_nitch][tmp_index] = cal_euclidean(pop.stra, stra_0);
			index[tmp_nitch]++;
		}
		sprintf_s(fname, "./csv/%d/Distance_nitch_%d_%d_%d(%d).csv", method_pop, method_pop, trial_pop, g*per, gene);
		CsvModules::csv_fwrite(fname, pop_dis);
	}
	return true;
}
void Match::output_re_csv(int Opp_trial) {
	char fname[50];
	if (cru_k == 0) {
		sprintf_s(fname, "./csv/%d/PopResult_%d_%d_%d_%d.csv", method_pop, method_pop, trial_pop, Opp_trial, gene);
	}if (0 < cru_k) {
		sprintf_s(fname, "./csv/%d/PopResult_%d_%d_%d_%d_%d.csv", method_pop, method_pop, trial_pop, Opp_trial, gene, cru_k);
	}
	//
	CsvModules::csv_fwrite(std::string(fname), eval, per);
}
