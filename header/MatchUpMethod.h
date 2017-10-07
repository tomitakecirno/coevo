#pragma once
#include "config.hpp"
#include <iostream>
#include <fstream>
#include <time.h>

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
			ToCsv_Data[i].resize(Pop_Length); //対戦相手数
		}
	}
	void Match_And_SetDATA(int Pop_Trial, int Opp_Trial);
	void File_Write_CSV(int Pop_trial, int Opp_trial);
	void Decide_Best(int Pop_Trial);
private:
	int Gene;	//世代数
	int Per;	//区切り
	int Loop_Length;	//ループ
	int Pop_Length;	//世代数
	int Opp_Length;
	int Pop_Method;	//世代数
	int Opp_Method;
	int	Cru_K;
	std::string Dir;
	std::vector<std::vector<std::vector<double> > > Pop_Result;
	std::vector<std::vector<double> > Opp_Result;
	std::vector<std::vector<double> > ToCsv_Data;
protected:
	void Set_CsvData(int trial);
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
				Pop_Result[AI_Pop][Gene][AI_Opp] = 1;
			}
			else {
				Pop_Result[AI_Pop][Gene][AI_Opp] = 0;
			}
			std::cout << Pop_Result[AI_Pop][Gene][AI_Opp];
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
void Match::Match_And_SetDATA(int Pop_Trial, int Opp_Trial) {
	for (int gene = 0; gene < Loop_Length; gene++) {
		std::cout << "generation:" << gene << std::endl;
		//対戦して結果を格納
		PvP(Pop_Trial, Opp_Trial, gene);
		//対戦結果から平均勝利数を求める
		Set_CsvData(gene);
	}
	std::cout << "Set Data" << std::endl;
}
void Match::Set_CsvData(int g) {
	//0で初期化
	std::vector<double> Pop_Eval(Pop_Length, 0);

	for (int p = 0; p < Pop_Length; p++) {
		std::cout << p << "_Result:[";
		for (int o = 0; o < Opp_Length; o++) {
			std::cout << Pop_Result[p][g][o] << ",";
		}
		std::cout << "]" <<std::endl;
	}
	//自プレイヤーの評価値を求める
	for (int p = 0; p < Pop_Length; p++) {
		for (int o = 0; o < Opp_Length; o++) {
			Pop_Eval[p] += Pop_Result[p][g][o];
			//std::cout << Pop_Eval[p] << ',';
		}
	}
	std::cout << "eval:[";
	for (int p = 0; p < Pop_Length; p++) {
		std::cout << Pop_Eval[p] << ",";
	}
	std::cout << "]" << std::endl;

	ToCsv_Data[g] = Pop_Eval;
}
void Match::File_Write_CSV(int Pop_trial, int Opp_trial) {
	char fname[50];
	if (Cru_K == 0) {
		sprintf(fname, "./csv/PopResult/%d/PopResult_%d_%d.csv", Pop_Method, Pop_trial, Opp_trial);
	}if (0 < Cru_K) {
		sprintf(fname, "./csv/PopResult/%d/PopResult_%d_%d_%d.csv", Pop_Method, Cru_K, Pop_trial, Opp_trial);
	}
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