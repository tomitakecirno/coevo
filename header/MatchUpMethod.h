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
		Gene = Gene_N + 1;	//世代数
		Per = Per_N;
		Loop_Length = (Gene_N / Per_N) + 1;
		Cru_K = Cru;

		Pop_Method = Method_P;
		Opp_Method = Method_O;

		Pop_Length = Pop_N;
		Opp_Length = Opp_N;
		//自プレイヤーの初期化
		Pop_Result = std::vector<std::vector<double>>(Pop_Length, std::vector<double>(Opp_Length,0));
		//相手プレイヤーの初期化
		Opp_Result = std::vector<std::vector<double>>(Opp_Length, std::vector<double>(Pop_Length,0));
		//Csvファイル出力用ベクターの初期化
		ToCsv_Data = std::vector<std::vector<double>>(Loop_Length, std::vector<double>(Pop_Length, 0));
	}
	void main_task(int Pop_Trial, int Opp_Trial);
	void output_re_csv(int Pop_trial, int Opp_trial);
	void output_ni_csv(int Pop_trial, int Opp_trial);
	void Decide_Best(int Pop_Trial);
private:
	int Gene;	//世代数
	int Per;	//区切り
	int Loop_Length;	//ループ
	int Pop_Length;	//世代数
	int Opp_Length;
	int Pop_Method;
	int Opp_Method;
	int	Cru_K;
	std::string Dir;
	std::vector<std::vector<double>> Pop_Result;
	std::vector<std::vector<double> > Opp_Result;
	std::vector<std::vector<double> > ToCsv_Data;
protected:
	void setdata(int trial);
	void PvP(int Pop_Trial, int Opp_Trial, int Gene);
};

//自プレイヤーと相手プレイヤーを戦わせる
void Match::PvP(int Pop_Trial, int Opp_Trial, int Gene) {
	for (int AI_Pop = 0; AI_Pop < Pop_Length; AI_Pop++) {
		//cout << ai1 << " ";
		char filename[50];
		if (Cru_K == 0) {
			sprintf_s(filename, ("%s/%d/%d/%d/%d.dat"), Dir.c_str(),Pop_Method, Pop_Trial, Gene, AI_Pop);
		}else if (0 < Cru_K) {
			sprintf_s(filename, ("%s/%d/%d/%d/%d/%d.dat"), Dir.c_str(),Pop_Method, Pop_Trial, Cru_K, Gene, AI_Pop);
		}
		if ((file = fopen(filename, "rb")) == NULL) {
			printf("Main file open error!!\n");
			exit(0);
		}
		else {
			if (0 < Pop_Method) {
				std::cout << "Cru:" << Cru_K << std::endl;
			}
			std::cout
				<< "open:Population_"
				<< Pop_Method << '_'
				<< Pop_Trial << '_'
				<< Gene << '_'
				<< AI_Pop << std::endl;
		}
		//自プレイヤーの戦略格納
		fread(w1, sizeof(double), I1 * J1, file);
		fread(w2, sizeof(double), I2 * J2, file);
		fread(w3, sizeof(double), I2 * J1, file);
		fclose(file);

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
			if (player1.win == 1) {
				Pop_Result[AI_Pop][AI_Opp] = 1;
			}
			else {
				Pop_Result[AI_Pop][AI_Opp] = 0;
			}
			std::cout << Pop_Result[AI_Pop][AI_Opp];
		}
	}
	std::cout << std::endl;

}
//集団内全数対戦
void Match::Decide_Best(int Pop_Trial) {
	std::vector<std::vector<double> > Tmp_Result(KO);
	for (int i = 0; i < KO; i++) {
		Tmp_Result[i].resize(KO);
	}
	for (int AI_Pop = 0; AI_Pop < Pop_Length; AI_Pop++) {
		//cout << ai1 << " ";
		char filename[50];
		if (Cru_K == 0) {
			sprintf_s(filename, ("%s/%d/%d/%d/%d.dat"), Dir.c_str(), Pop_Method, Pop_Trial, Gene, AI_Pop);
		}
		else if (0 < Cru_K) {
			sprintf_s(filename, ("%s/%d/%d/%d/%d/%d.dat"), Dir.c_str(), Pop_Method, Pop_Trial, Cru_K, Gene, AI_Pop);
		}
		if ((file = fopen(filename, "rb")) == NULL) {
			printf("file open error_1!!\n");
			exit(0);
		}
		else {
			if (0 < Pop_Method) {
				std::cout << "Cru:" << Cru_K << std::endl;
			}
			std::cout
				<< "open:Population_"
				<< Pop_Method << '_'
				<< Pop_Trial << '_'
				<< Gene << '_'
				<< AI_Pop << std::endl;
		}
		//自プレイヤーの戦略格納
		fread(w1, sizeof(double), I1 * J1, file);
		fread(w2, sizeof(double), I2 * J2, file);
		fread(w3, sizeof(double), I2 * J1, file);
		fclose(file);

		//対戦相手
		for (int AI_Pop_2 = AI_Pop; AI_Pop_2 < Pop_Length; AI_Pop_2++) {
			if (Cru_K == 0) {
				sprintf_s(filename, ("%s/%d/%d/%d/%d.dat"), Dir.c_str(), Pop_Method, Pop_Trial, Gene, AI_Pop);
			}
			else if (0 < Cru_K) {
				sprintf_s(filename, ("%s/%d/%d/%d/%d/%d.dat"), Dir.c_str(), Pop_Method, Pop_Trial, Cru_K, Gene, AI_Pop);
			}
			if ((file = fopen(filename, "rb")) == NULL) {
				printf("file open error_2!!\n");
				exit(0);
			}
			//相手プレイヤーの戦略格納
			fread(w1_T, sizeof(double), I1 * J1, fp);
			fread(w2_T, sizeof(double), I2 * J2, fp);
			fread(w3_T, sizeof(double), I2 * J1, fp);
			fclose(fp);

			//対戦
			Competition();
			Tmp_Result[AI_Pop][AI_Pop_2] = (player1.hp - player2.hp) / 300;
			Tmp_Result[AI_Pop_2][AI_Pop] = (player2.hp - player1.hp) / 300;
		}
	}
	//最良個体を決定
	std::vector<double> Tmp_Eval;
	Tmp_Eval.assign(KO, 0);
	for (int i = 0; i < KO; i++) {
		for (int j = 0; j < KO; j++) {
			Tmp_Eval[i] += Tmp_Result[i][j];
		}
	}
	auto Max = max_element(Tmp_Eval.begin(), Tmp_Eval.end());
	int Max_Index = int( distance(Tmp_Eval.begin(), Max) );

	//最良個体を読み込んで，Bestとして保存する
	if (Pop_Method == 0) {
		sprintf_s(filename, ("AI/%d/%d/%d/%d.dat"), Pop_Method, Pop_Trial, Gene, Max_Index);
	}
	else if (0 < Pop_Method) {
		sprintf_s(filename, ("AI/%d/%d/%d/%d/%d.dat"), Pop_Method, Pop_Trial, Cru_K, Gene, Max_Index);
	}
	if ((file = fopen(filename, "rb")) == NULL) {
		printf("file open error_3!!\n");
		exit(0);
	}
	fread(w1, sizeof(double), I1 * J1, file);
	fread(w2, sizeof(double), I2 * J2, file);
	fread(w3, sizeof(double), I2 * J1, file);
	fclose(file);

	if (Pop_Method == 0) {
		sprintf_s(filename, ("AI/%d/%d/%d/Best.dat"), Pop_Method, Pop_Trial, Gene);
	}
	else if (0 < Pop_Method) {
		sprintf_s(filename, ("AI/%d/%d/%d/%d/Best.dat"), Pop_Method, Pop_Trial, Cru_K, Gene);
	}
	if ((file = fopen(filename, "rb")) == NULL) {
		printf("file open error_4!!\n");
		exit(0);
	}
	fwrite(w1, sizeof(double), I1*J1, fp);
	fwrite(w2, sizeof(double), I2*J2, fp);
	fwrite(w3, sizeof(double), I2*J1, fp);

}
void Match::main_task(int Pop_Trial, int Opp_Trial)
{
	for (int gene = 0; gene < Loop_Length; gene++) {
		std::cout << "generation:" << gene << std::endl;
		//対戦して結果を格納
		PvP(Pop_Trial, Opp_Trial, gene);
		//対戦結果から平均勝利数を求める
		setdata(gene);
	}
	std::cout << "Set Data" << std::endl;
	output_re_csv(Pop_Trial, Opp_Trial);
	output_ni_csv(Pop_Trial, Opp_Trial);
	std::cout << "Output result csv" << std::endl;
}
void Match::setdata(int g) {
	//評価値を求める
	std::vector<double> pop_eval(Pop_Length, 0);
	for (auto &p : Pop_Result) {
		std::cout << "Result:[";
		for (auto &o : p) {
			std::cout << o << ",";
		}
		std::cout << "]" <<std::endl;
	}
	for (int i = 0; i < Pop_Length; i++) {
		for (auto &pr : Pop_Result[i]) {
			pop_eval[i] += pr;
		}
	}
	std::cout << "eval:[";
	for (auto &pe : pop_eval) {
		std::cout << pe << ",";
	}
	std::cout << "]" << std::endl;
	ToCsv_Data[g] = pop_eval;
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
void Match::output_ni_csv(int Pop_trial, int Opp_trial) {
	CsvModules cm;
	char fname[50];
	std::vector<std::vector<int>> pop_nitch; //ニッチ番号を格納

	if (Cru_K == 0) {
		sprintf(fname, "./csv/%d/Cruster_%d_%d_%d.csv", Pop_Method, Pop_Method, Pop_trial, Gene);
	}if (0 < Cru_K) {
		sprintf(fname, "./csv/%d/PopResult_%d_%d_%d_%d_%d.csv", Pop_Method, Pop_Method, Pop_trial, Opp_trial, Gene, Cru_K);
	}

	if (!cm.GetContents(fname, pop_nitch)) {
		std::cout << "pop_nitch input error" << std::endl;
	}

	for (int i = 0; i < Loop_Length; i++) {
		int max_num = *max_element(pop_nitch[i].begin(), pop_nitch[i].end());
		for (int j = 0; j < max_num; j++) {
			int count = int(std::count(pop_nitch[i].begin(), pop_nitch[i].end(), j));
			if (count != 0) {
				//ニッチ毎の個体を記録
				int c_index = 0;
				std::vector<std::vector<double>> tmp_eval;
				tmp_eval = std::vector<std::vector<double>>(count, std::vector<double>(Opp_Length, 0));
				for (int k = 0; k < Pop_Length; k++) {
					if (pop_nitch[i][k] == j) {
						tmp_eval[c_index] = Pop_Result[i];
						c_index++;
					}
				}
				//ファイル出力
				char fname[50];
				if (Cru_K == 0) {
					sprintf(fname, "./csv/%d/nrdata/pop_nitch_result_%d_%d_%d_%d_%d.csv", Pop_Method, Pop_Method, Pop_trial, Opp_trial, Gene,j);
				}if (0 < Cru_K) {
					sprintf(fname, "./csv/%d/nrdata/pop_nitch_result_%d_%d_%d_%d_%d_%d.csv", Pop_Method, Pop_Method, Pop_trial, Opp_trial, Gene, Cru_K,j);
				}
				cm.csv_fwrite(std::string(fname), tmp_eval);
			}
		}
		//
	}
}