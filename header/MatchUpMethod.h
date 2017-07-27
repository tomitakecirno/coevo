#pragma once
#include "config.hpp"
#include <iostream>
#include <fstream>
#include <time.h>

class Match {
public:
	void Init_Parameter(int Method_P, int Method_O, int Pop_N, int Opp_N, int Gene_N, int Per_N, int Cru = 0);
	void Match_And_SetDATA(int Pop_Trial, int Opp_Trial);
	void File_Write_CSV(int Pop_trial, int Opp_trial);
private:
	int Gene;	//世代数
	int Per;	//区切り
	int Loop_Length;	//ループ
	int Pop_Length;	//世代数
	int Opp_Length;
	int Pop_Method;	//世代数
	int Opp_Method;
	int	Cru_K;
	std::vector<std::vector<std::vector<double> > > Pop_Result;
	std::vector<std::vector<double> > Opp_Result;
	std::vector<std::vector<double> > ToCsv_Data;
protected:
	void Set_CsvData(int trial);
	void PvP(int Pop_Trial, int Opp_Trial, int Gene);
};

void Match::Init_Parameter(int Method_P, int Method_O, int Pop_N, int Opp_N, int Gene_N, int Per_N, int Cru) {
	Gene = Gene_N;	//世代数
	Per = Per_N;
	Loop_Length = Gene_N / Per_N;
	Cru_K = Cru;

	Pop_Method = Method_P;
	Opp_Method = Method_O;

	Pop_Length = Pop_N;	//世代数
	Opp_Length = Opp_N;

	//自プレイヤーの初期化
	Pop_Result.resize(Pop_Length); //メンバー数
	for (int i = 0; i < Pop_Length; i++) {
		Pop_Result[i].resize(Loop_Length); //世代数
		for (int j = 0; j < Loop_Length; j++) {
			Pop_Result[i][j].resize(Opp_Length); //対戦相手数
		}
	}
	//相手プレイヤーの初期化
	Opp_Result.resize(Opp_Length);
	for (int i = 0; i < Opp_Length; i++) {
		Opp_Result[i].resize(Pop_Length); //対戦相手数
	}
	//Csvファイル出力用ベクターの初期化
	ToCsv_Data.resize(Loop_Length);
	for (int i = 0; i < Loop_Length; i++) {
		ToCsv_Data.resize(Pop_Length); //対戦相手数
	}
}
//自プレイヤーと相手プレイヤーを戦わせる
void Match::PvP(int Pop_Trial, int Opp_Trial, int Gene) {
	for (int AI_Pop = 0; AI_Pop < Pop_Length; AI_Pop++) {
		//cout << ai1 << " ";
		char filename[50];
		if (Pop_Method == 0) {
			sprintf_s(filename, ("AI/%d/%d/%d/%d.dat"), Pop_Method, Pop_Trial, Gene, AI_Pop);
		}else if (Pop_Method == 1) {
			sprintf_s(filename, ("AI/%d/%d/%d/%d/%d.dat"), Pop_Method, Pop_Trial, Cru_K, Gene, AI_Pop);
		}
		if ((file = fopen(filename, "rb")) == NULL) {
			printf("file open error!!\n");
			exit(0);
		}
		else {
			if (Pop_Method == 1) {
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
				Pop_Result[AI_Pop][Gene][AI_Opp] = 1;
			}
			else if (player2.win == 1) {
				Pop_Result[AI_Pop][Gene][AI_Opp] = 0;
			}
			else {
				Pop_Result[AI_Pop][Gene][AI_Opp] = 0;
			}
		}
	}

}
void Match::Match_And_SetDATA(int Pop_Trial, int Opp_Trial) {
	for (int gene = 0; gene < Loop_Length; gene++) {
		std::cout << "generation:" << gene << std::endl;
		clock_t start = clock();
		//対戦して結果を格納
		PvP(Pop_Trial, Opp_Trial, gene);
		//対戦結果から平均勝利数を求める
		Set_CsvData(gene);
		clock_t end = clock();

		int time = (end - start) / CLOCKS_PER_SEC;
		int hour = time / 3600;
		int minute = time / 3600 / 60;
		int second = time % 60;
		std::cout << "Process time:" << std::oct << std::showbase << hour << ':';
		std::cout << std::oct << std::showbase << minute << ':';
		std::cout << std::oct << std::showbase << second << std::endl;
	}
	std::cout << "Set Data" << std::endl;
}
void Match::Set_CsvData(int g) {
	//0で初期化
	std::vector<double> Pop_Eval(Pop_Length, 0);
	//0で初期化
	//自プレイヤーの評価値を求める
	for (int p = 0; p < Pop_Length; p++) {
		for (int o = 0; o < Opp_Length; o++) {
			Pop_Eval[p] += Pop_Result[p][g][o];
			//std::cout << Pop_Eval[p] << ',';
		}
	}
	//std::cout << std::endl;
	ToCsv_Data[g] = Pop_Eval;
}
void Match::File_Write_CSV(int Pop_trial, int Opp_trial) {
	char fname[50];
	sprintf(fname, "./csv/PopResult/PopResult_%d_%d_%d.csv", Pop_Method, Pop_trial, Opp_trial);
	//ファイル出力ストリーム
	std::ofstream ofs(fname);

	ofs << "世代数" << ',';
	for (int j = 0; j < Pop_Length; j++) {
		ofs << "p" << j << ',';
	}
	ofs << std::endl;

	//結果を出力
	for (int i = 0; i < Loop_Length; i++) {
		ofs << i*Per << ',';
		for (int j = 0; j < Pop_Length; j++) {
			ofs << ToCsv_Data[i][j] << ',';
			//std::cout << ToCsv_Data[i][j] << ',';
		}
		ofs << std::endl;
	}
	std::cout << "File Written" << std::endl;
}