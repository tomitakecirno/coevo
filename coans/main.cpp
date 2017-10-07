#pragma once
#include "../header/coansmodule.hpp"
#include "../header/CoansMethods.h"
#include "../header/MatchUpMethod.h"
#include "../header/Floreano.h"
#include "../header/config.hpp"
#include <fstream>
#include <time.h>
#include <cstdlib>
#include <cstring>

using namespace std;

/****************************
メインルーチン
DE
0:対戦相手学習
1:プレイヤーVSプレイヤー
2:ANSクラスタリング手法学習
3:階層的クラスタリング手法
***************************/
void MatchUp_CSV(std::vector<int> &Count);
void Show_Time(clock_t Start, clock_t End);
void Init_Test();

/***********************************************
PARAMETER
Main_Mode	:モード	(0: 1: 2:提案手法の学習 3:)
Main_Method	:提案手法
Main_Trial	:試行回数
Main_K		:クラスタリングパラメーター(?)
***********************************************/
int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int Main_Mode;
	int Main_Trial;
	int Main_K;
	int	Main_Method;

	/*
		argv[1]:Method
		argv[2]:Trial
		argv[3]:k
	*/
	if (__argc < 3) {
		//パラメーター無しで動かす用
		Main_Mode = 3;
		Main_Method = 2;
		Main_Trial = 0;
		Main_K = 0;
	}else if(__argc == 4) {
		Main_Mode = atoi(__argv[1]);
		Main_Method = atoi(__argv[2]);
		Main_Trial = atoi(__argv[3]);
		Main_K = 0;
	}
	else if (__argc == 5) {
		Main_Mode = atoi(__argv[1]);
		Main_Method = atoi(__argv[2]);
		Main_Trial = atoi(__argv[3]);
		Main_K = atoi(__argv[4]);
	}

	std::cout << "Mode:" << Main_Mode << std::endl;
	std::cout << "手法:" << Main_Method << std::endl;
	std::cout << "試行回数:" << Main_Trial << std::endl;
	std::cout << "クラスタ数:" << Main_K << std::endl;
	std::cout << "世代数:" << KU << std::endl;
	std::cout << "区切り:" << PER << std::endl;

	int MatchUp_Count=0;

	clock_t Start_Main = clock();
	//実験用対戦相手学習
	if (Main_Mode == 0) {
		Make_Directory_AIT(0,TRIAL);
		FloreMethods Flore_1;
		for (int t = 4; t < TRIAL; t++) {
			Flore_1.Main_Tasks();
			Flore_1.Fwrite_Floreano(t);
		}
	}
	//学習で記録したデータをもとに実際に対戦
	else if (Main_Mode == 1) {
		Match Match_0("AI", Main_Method, 0, KO, KOT, KU, PER, Main_K);
		Make_CSV_Directory(Main_Method);

		for (int Opp_t = 0; Opp_t < F_TRIAL; Opp_t++) {
			//現手法vsFloreano 世代数:2000(100世代間隔) 現手法集団50 Floreano集団30
			Match_0.Match_And_SetDATA(Main_Trial, Opp_t);
			Match_0.File_Write_CSV(Main_Trial, Opp_t);
		}
		std::cout << "Process time:" << time << std::endl;
		Match_0.Decide_Best(Main_Trial);
	}

	//現手法学習
	else if (Main_Mode == 2) {
		//現手法
		if (Main_Method == 0) {
			Coans_GT2016 Coans_GT2016("2016");
			Coans_GT2016.Coans_GT2016_Tasks(Main_Trial);
			MatchUp_Count = Coans_GT2016.Get_MatchUp_Num();
		}
		//階層的クラスタリングを盛り込んだ手法
		if (Main_Method == 1) {
			CoansMode1 Mode1("AI");
			Mode1.Coans_Tasks(Main_Trial);
			MatchUp_Count = Mode1.Get_MatchUp_Num();
		}
		//階層的クラスタリング＋List3
		else if (Main_Method == 2) {
			CoansMode2 Mode2("AI");
			Mode2.Coans_Tasks(Main_Trial);
			MatchUp_Count = Mode2.Get_MatchUp_Num();
		}else if (Main_Method == 3) {
			CoansMode3 Mode3("AI");
			Mode3.Coans_Tasks(Main_Trial);
			MatchUp_Count = Mode3.Get_MatchUp_Num();
		}
		else if (Main_Method == 4) {
			CoansMode4 Mode4("AI");
			Mode4.Coans_Tasks(Main_Trial);
			MatchUp_Count = Mode4.Get_MatchUp_Num();
		}
	}
	//csv統合
	else if (Main_Mode == 3) {
		CsvModules_Intend Method;
		Method.Create_Data(Main_Method, KO, Main_Trial, KU, PER);
	}
	//テスト
	else if (Main_Mode == 4) {
		Init_Test();
		gamemode();
	}

	clock_t End_Main = clock();
	Show_Time(Start_Main, End_Main);
	std::cout << "match_up num:" << MatchUp_Count << std::endl;

	//プレイ
	//WaitKey();		// キー入力待ち
	DxLib_End();			// ＤＸライブラリ使用の終了処理
	return 0;				// ソフトの終了 
}

void Show_Time(clock_t Start, clock_t End) {
	int time = (Start - Start) / CLOCKS_PER_SEC;
	std::cout << "Process time:" << time << "[sec]" << std::endl;
}


void MatchUp_CSV(std::vector<int> &Count) {
	//ファイル名設定
	char fname[30];
	sprintf(fname, "MatchUp_Log.csv");

	//ファイル出力ストリーム
	std::ofstream ofs(fname);
	for (int i = 0; i < TRIAL; i++) {
		ofs << Count[i] << ',';
	}
	ofs << std::endl;
}

void Init_Test() {
	double Test_W1[I1][J1];//1P
	double Test_W2[I2][J2];
	double Test_W3[I2][J1];

	for (int i = 0; i < I1; i++) {
		for (int j = 0; j < J1; j++) {
			Test_W1[i][j] = GetRand_Real(10);
		}
	}
	for (int i = 0; i < I2; i++) {
		for (int j = 0; j < J2; j++) {
			Test_W2[i][j] = GetRand_Real(10);
		}
	}
	for (int i = 0; i < J1; i++) {
		for (int j = 0; j < I2; j++) {
			Test_W3[i][j] = GetRand_Real(10);
		}
	}

	//AIフォルダを作る
	std::stringstream File_Name;
	FILE *fp_test;

	File_Name << "./AI";
	CheckTheFolder::checkExistenceOfFolder(File_Name.str());

	//ファイルオープン
	//test.datへ書き込み
	if ((fp_test = fopen("./AI/test.dat", "wb+")) == NULL) {
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	fwrite(Test_W1, sizeof(double), I1*J1, fp_test);
	fwrite(Test_W2, sizeof(double), I2*J2, fp_test);
	fwrite(Test_W3, sizeof(double), I2*J1, fp_test);
	fclose(fp_test);

	//test_T.datへ書き込み
	if ((fp_test = fopen("./AI/test_T.dat", "wb+")) == NULL) {
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	fwrite(Test_W1, sizeof(double), I1*J1, fp_test);
	fwrite(Test_W2, sizeof(double), I2*J2, fp_test);
	fwrite(Test_W3, sizeof(double), I2*J1, fp_test);
	fclose(fp_test);
}