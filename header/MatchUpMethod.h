#pragma once
#include "config.hpp"
#include <iostream>
#include <fstream>
#include <time.h>
#include "CsvModules.h"

class Match {
public:
	Match(std::string dir, int Method_P, int Method_O, int Pop_N, int Opp_N, int Gene_N, int Per_N, int Cru = 0) {
		Dir = dir;
		Gene = Gene_N;	//世代数
		Per = Per_N;
		Loop_Length = (Gene_N / Per_N) + 1;
		Cru_K = Cru;

		Pop_Method = Method_P;
		Opp_Method = Method_O;

		Pop_Length = Pop_N;
		Opp_Length = Opp_N;

		Pop.resize(Pop_N);
		//自プレイヤーの初期化
		for (int i = 0; i < Pop_Length; i++) {
			Pop[i].Init();
			Pop[i].Init_0();
		}
		//Csvファイル出力用ベクターの初期化
		ToCsv_Data = std::vector<std::vector<double>>(Loop_Length, std::vector<double>(Pop_Length, 0));
	}
	void main_task(int Pop_Trial, int Opp_Trial);
	void output_re_csv(int Pop_trial, int Opp_trial);
	bool output_ni_csv(int Pop_trial);
private:
	int Gene;			//世代数
	int Per;			//区切り
	int Loop_Length;	//ループ
	int Pop_Length;		//世代数
	int Opp_Length;
	int Pop_Method;
	int Opp_Method;
	int	Cru_K;
	std::string Dir;
	std::vector<playerTK_ex> Pop;
	std::vector<std::vector<double> > ToCsv_Data;
protected:
	void setdata(int trial);
	void input_stra(int Pop_Trial,int Gene);
	void PvP(int Pop_Trial, int Opp_Trial, int Gene);
};

//自プレイヤーと相手プレイヤーを戦わせる
void Match::input_stra(int Pop_Trial, int Gene) 
{
	for (int i = 0; i < Pop_Length; i++) {
		char filename[50];
		if (Cru_K == 0) {
			sprintf_s(filename, ("%s/%d/%d/%d/%d.dat"), Dir.c_str(), Pop_Method, Pop_Trial, Gene, i);
		}
		else if (0 < Cru_K) {
			sprintf_s(filename, ("%s/%d/%d/%d/%d/%d.dat"), Dir.c_str(), Pop_Method, Pop_Trial, Cru_K, Gene, i);
		}
		if ((file = fopen(filename, "rb")) == NULL) {
			std::cout << "file open error :" << filename << std::endl;
			exit(0);
		}
		//自プレイヤーの戦略格納
		fread(w1_inout, sizeof(double), I1 * J1, file);
		fread(w2_inout, sizeof(double), I2 * J2, file);
		fread(w3_inout, sizeof(double), I2 * J1, file);
		fclose(file);
		w_to_Strategy(Pop[i]);
	}
}
void Match::PvP(int Pop_Trial, int Opp_Trial, int Gene)
{
	input_stra(Pop_Trial, Gene); //集団の戦略をインプット
	for (int AI_Pop = 0; AI_Pop < Pop_Length; AI_Pop++) {
		//cout << ai1 << " ";
		StrategySet_M(Pop[AI_Pop]);
		Pop[AI_Pop].Result.resize(Opp_Length);
		Pop[AI_Pop].Result_int.resize(Opp_Length);
		//対戦相手
		for (int AI_Opp = 0; AI_Opp < Opp_Length; AI_Opp++) {
			sprintf(filename, ("AIT/%d/%d/%d.dat"), Opp_Method, Opp_Trial, AI_Opp);
			if ((fp = fopen(filename, "rb")) == NULL) {
				printf("file open error!!\n");
				exit(0);
			}
			//相手プレイヤーの戦略格納
			fread(w1_T, sizeof(double), I1 * J1, fp);
			fread(w2_T, sizeof(double), I2 * J2, fp);
			fread(w3_T, sizeof(double), I2 * J1, fp);
			fclose(fp);

			//対戦
			Competition();
			Pop[AI_Pop].Result[AI_Opp] = player1.hp - player2.hp;
			if (player1.win == 1) {
				Pop[AI_Pop].Result_int[AI_Opp] = 1;
			}
			else {
				Pop[AI_Pop].Result_int[AI_Opp] = 0;
			}
		}
	}
}
void Match::main_task(int Pop_Trial, int Opp_Trial)
{
	for (int gene = 0; gene < Loop_Length; gene++) {
		std::cout << "Opp:" << Opp_Trial << "  Generation:" << gene << "   ";
		//対戦して結果を格納
		PvP(Pop_Trial, Opp_Trial, gene);
		//対戦結果から平均勝利数を求める
		setdata(gene);
	}
	std::cout << "Set Data" << std::endl;
	output_re_csv(Pop_Trial, Opp_Trial);
	std::cout << "Output result csv" << std::endl;
}
void Match::setdata(int g) {
	//評価値を求める
	std::vector<double> pop_eval(Pop_Length, 0);
	/*
	for (auto &p : Pop_Result) {
		std::cout << "Result:[";
		for (auto &o : p) {
			std::cout << o << ",";
		}
		std::cout << "]" <<std::endl;
	}
	*/
	for (int i = 0; i < Pop_Length; i++) {
		for (auto &pr : Pop[i].Result) {
			if (pr > 0) {
				pop_eval[i] += 1;
			}
			else {
				pop_eval[i] += 0;
			}
		}
	}
	std::cout << "eval:[";
	for (auto &pe : pop_eval) {
		std::cout << pe << ",";
	}
	std::cout << "]" << std::endl;
	ToCsv_Data[g] = pop_eval;
}
bool Match::output_ni_csv(int Pop_trial) 
{
	std::cout << "output_ni_csv..." << std::endl;
	CsvModules csv;
	std::vector<std::vector<int>> pop_nitch;
	char fname[50];
	if (Cru_K == 0) {
		sprintf(fname, "./csv/%d/Cruster_%d_%d_%d.csv", Pop_Method, Pop_Method, Pop_trial, Gene);
	}if (0 < Cru_K) {
		sprintf(fname, "./csv/%d/Cruster_%d_%d_%d_%d.csv", Pop_Method, Pop_Method, Pop_trial, Gene, Cru_K);
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
	for (int g = 0; g < Gene / Per + 1; g++) {
		input_stra(Pop_trial, g);

		int max = *max_element(pop_nitch[g].begin(), pop_nitch[g].end());
		std::vector<int> index(max+1, 0);
		std::vector<std::vector<double>> pop_dis(max+1);
		std::vector<std::vector<double>> pop_res(max+1);

		for (int i = 0; i < max+1; i++) {
			int count = int(std::count(pop_nitch[g].begin(), pop_nitch[g].end(), i));
			pop_dis[i].resize(count);
		}

		//戦略間距離
		playerTK a_pop;
		a_pop.Init_0();

		for (int i = 0; i < Pop_Length; i++) {
			int tmp_nitch = pop_nitch[g][i];
			int tmp_index = index[tmp_nitch];
			pop_dis[tmp_nitch][tmp_index] = cal_kotai_distance(Pop[i], a_pop);
			index[tmp_nitch]++;
		}
		sprintf(fname, "./csv/%d/Distance_nitch_%d_%d_%d(%d).csv", Pop_Method, Pop_Method, Pop_trial, g*Per, Gene);
		csv.csv_fwrite(fname, pop_dis);
	}
	return true;
}
void Match::output_re_csv(int Pop_trial, int Opp_trial) {
	char fname[50];
	if (Cru_K == 0) {
		sprintf(fname, "./csv/%d/PopResult_%d_%d_%d_%d.csv", Pop_Method, Pop_Method, Pop_trial, Opp_trial, Gene);
	}if (0 < Cru_K) {
		sprintf(fname, "./csv/%d/PopResult_%d_%d_%d_%d_%d.csv", Pop_Method, Pop_Method, Pop_trial, Opp_trial, Gene, Cru_K);
	}
	//
	CsvModules cm;
	cm.csv_fwrite(std::string(fname), ToCsv_Data, Per);
}
