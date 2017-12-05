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
	int Main_Mode	= 1;
	int	Main_Method = 2;
	int Main_Trial	= 0;
	int Main_K		= 0;
	int Main_KU		= KU;
	int Main_PER	= PER;
	int Main_KO		= KO;
	int	Main_PARENT = PARENT;
	int	Main_CHILD	= CHILD;

	std::vector<int> method;
	/*
		argv[0]:.exe
		argv[1]:mode
		argv[2]:method
		argv[3]:generation
		argv[4]:trials
		argv[5]:parent
		argv[6]:child
		argv[7]:nitch parameter
		*/
	Main_Mode = 2;
	//Main_Mode = atoi(__argv[1]);
	
	if (Main_Mode == 1 || Main_Mode == 2 || Main_Mode == 4) {
		switch (__argc) {
		case 1:
			//Debug mode
			Main_Method = 2;
			Main_KU		= 1;
			Main_Trial	= 0;
			Main_PARENT = 4;
			Main_CHILD	= 10;
			break;
		case 4:
			Main_Method = atoi(__argv[2]);
			Main_KU = atoi(__argv[3]);
			break;
		case 5:
			Main_Method = atoi(__argv[2]);
			Main_KU = atoi(__argv[3]);
			Main_Trial = atoi(__argv[4]);
			break;
		default:
			break;
		}
		//Main_PER = Main_KU / 10;
		if (Main_Mode == 1) {
			std::cout << "モード:対戦" << std::endl;
		}
		else if (Main_Mode == 2) {
			std::cout << "モード:学習"  << std::endl;
		}
	}
	if (Main_Mode == 3) {
		//method.resize(1);
		//method[0] = 2;
		method.resize(__argc-2);
		for (int i = 0; i < __argc - 2; i++) {
			method[i] = atoi(__argv[i+2]);
			//method[i] = 2;
		}
		std::cout << "モード:csv統合"  << std::endl;
	}
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
		Match m("AI", Main_Method, 0, Main_KO, KOT, Main_KU, Main_PER, Main_K);
		Make_CSV_Directory(Main_Method);
		if (Main_Method == 2) {
			if (!m.output_ni_csv(Main_Trial)) {
				std::cout << "error : output_ni_csv" << std::endl;
				exit(0);
			}
		}
		for (int Opp_t = 0; Opp_t < 5; Opp_t++) {
			//現手法vsFloreano 世代数:2000(100世代間隔) 現手法集団50 Floreano集団30
			m.main_task(Main_Trial, Opp_t);
		}
	}

	//現手法学習
	// DIR, KO, KU, PER, K, PARENT, CHILD
	else if (Main_Mode == 2) {
		if (Main_Method == 1) {
			CoansMode1 Mode1("AI", Main_KO, Main_KU, Main_PER, Main_PARENT, Main_CHILD, Main_K);
			Mode1.Coans_Tasks(Main_Trial);
			MatchUp_Count = Mode1.Get_MatchUp_Num();
		}
		//階層的クラスタリング＋List3
		else if (Main_Method == 2) {
			CoansMode2 Mode2("AI", Main_KO, Main_KU, Main_PER, Main_PARENT, Main_CHILD, Main_K);
			Mode2.Coans_Tasks(Main_Trial);
			MatchUp_Count = Mode2.Get_MatchUp_Num();
		}else if (Main_Method == 3) {
			CoansMode3 Mode3("AI", Main_KO, Main_KU, Main_PER, Main_PARENT, Main_CHILD, Main_K);
			Mode3.Coans_Tasks(Main_Trial);
			MatchUp_Count = Mode3.Get_MatchUp_Num();
		}
		else if (Main_Method == 4) {
			CoansMode4 Mode4("AI", Main_KO, Main_KU, Main_PER, Main_K, Main_PARENT, Main_CHILD);
			Mode4.Coans_Tasks(Main_Trial);
			MatchUp_Count = Mode4.Get_MatchUp_Num();
		}
	}
	//csv統合
	else if (Main_Mode == 3) {
		Csv_exp Method;
		Method.integration(method, Main_KU, Main_PER);
	}
	//テスト
	else if (Main_Mode == 4) {
		if (Main_Method == 1) {
			CoansMode1 Mode1("AI", Main_KO, Main_KU, Main_PER, Main_PARENT, Main_CHILD);
			Mode1.Stra_nitch_CSV(Main_Trial);
		}
		if (Main_Method == 2) {
			CoansMode2 Mode2("AI", Main_KO, Main_KU, Main_PER, Main_PARENT, Main_CHILD);
			Mode2.Stra_nitch_CSV(Main_Trial);
		}
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