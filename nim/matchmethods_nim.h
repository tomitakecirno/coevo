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
	}
	void main_task(int Opp_Trial);
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
	std::vector<std::vector<double>> eval;
	void PvP(int Opp_Trial, int Gene);
	void output_re_csv(int Opp_trial);
	bool output_ni_csv();
};

void Match::PvP(int Opp_Trial, int g)
{
	nim nim;
	p_data pop,opp;
	pop.Init();
	opp.Init();
	char fname[50], fname2[50];
	for (int ai_pop = 0; ai_pop < KO; ai_pop++) {
		//プレイヤーの戦略読み込み
		sprintf_s(fname, "./nim/%d/%d/%d/%d.dat", method_pop, trial_pop, g, ai_pop);
		pop.input_stra(fname);
		pop.Result.assign(KOT, 0);
		for (int ai_opp = 0; ai_opp < KOT; ai_opp++) {
			//対戦相手の戦略読み込み
			sprintf_s(fname2, "AIT/%d/%d/%d.dat", method_opp, Opp_Trial, ai_opp);
			opp.input_stra(fname2);

			//1回目
			nim.input_stra_first(pop.stra);	//先手
			nim.input_stra_last(opp.stra);	//後手
			if (nim.nim_game()) {
				pop.Result[ai_opp] += 1*WIN_FIRST;
			}
			else {
				pop.Result[ai_opp] += 0;
			}

			//先手後手を入れ替えて2回目
			nim.input_stra_first(opp.stra);	//先手
			nim.input_stra_last(pop.stra);	//後手
			if (nim.nim_game()) {
				pop.Result[ai_opp] += 1 * WIN_LAST;
			}
			else {
				pop.Result[ai_opp] += 0;
			}
		}
		eval[g][ai_pop] = std::accumulate(pop.Result.begin(), pop.Result.end(), 0.0);
	}
}
void Match::main_task(int Opp_Trial)
{
	for (int g = 0; g < loop; g++) {
		std::cout << "Opp:" << Opp_Trial << "  Generation:" << g << "   " << std::endl;;
		//対戦して結果を格納
		PvP(Opp_Trial, g);
	}
	std::cout << "Set Data" << std::endl;
	output_re_csv(Opp_Trial);
	std::cout << "Output result csv" << std::endl;
}
bool Match::output_ni_csv() 
{
	std::cout << "output_ni_csv..." << std::endl;
	CsvModules csv;
	std::vector<std::vector<int>> pop_nitch;
	char fname[50];
	if (cru_k == 0) {
		sprintf_s(fname, "./csv/%d/Cruster_%d_%d_%d.csv", method_pop, method_pop, trial_pop, gene);
	}if (0 < cru_k) {
		sprintf_s(fname, "./csv/%d/Cruster_%d_%d_%d_%d.csv", method_pop, method_pop, trial_pop, gene, cru_k);
	}
	if (!csv.GetContents(fname, pop_nitch)) {
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
		std::vector<int> stra_0;
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

			pop_dis[tmp_nitch][tmp_index] = cal_haming(pop.stra, stra_0);
			index[tmp_nitch]++;
		}
		sprintf_s(fname, "./csv/%d/Distance_nitch_%d_%d_%d(%d).csv", method_pop, method_pop, trial_pop, g*per, gene);
		csv.csv_fwrite(fname, pop_dis);
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
	CsvModules cm;
	cm.csv_fwrite(std::string(fname), eval, per);
}
