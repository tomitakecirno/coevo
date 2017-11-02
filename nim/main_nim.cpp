#pragma once
#include <fstream>
#include <time.h>
#include <cstdlib>
#include <cstring>
#include "nim.h"
#include "coans_nim.h"
#include "coansmodule_nim.hpp"

using namespace std;

/****************************
メインルーチン
DE
0:対戦相手学習
1:プレイヤーVSプレイヤー
2:ANSクラスタリング手法学習
3:階層的クラスタリング手法
***************************/

void Show_Time(clock_t Start, clock_t End);
/***********************************************
PARAMETER
Main_Mode	:モード	(0: 1: 2:提案手法の学習 3:)
Main_Method	:提案手法
Main_Trial	:試行回数
Main_K		:クラスタリングパラメーター(?)
***********************************************/
int main(int argc, char *argv[])
{
	int mode;
	int	method;
	int ku;
	int trial;
	int per;
	int cru_k = 0;

	std::vector<int> method_vec;
	/*
		argv[0]:.exe
		argv[1]:mode
		argv[2]:method
		argv[3]:generation
		argv[4]:trial
		argv[5]:nitch parameter	
	*/
	mode = 2;
	//mode = atoi(__argv[1]);
	if (mode == 1 || mode == 2 || mode == 4) {
		switch (__argc) {
		case 1:
			//Debug mode
			method	= 1;
			ku		= 10;
			trial	= 0;
			break;
		case 5:
			method = atoi(__argv[2]);
			ku	   = atoi(__argv[3]);
			trial  = atoi(__argv[4]);
			break;
		default:
			cout << "パラメーターが足りません" << endl;
			cout << "coans.exe [mode][method][generation][trial]" << endl;
			exit(1);
			break;
		}
		per = ku / 10;
		if (mode == 1) {
			std::cout << "モード:対戦" << std::endl;
		}
		else if (mode == 2) {
			std::cout << "モード:学習"  << std::endl;
		}
	}
	if (mode == 3) {
		//method.resize(1);
		//method[0] = 2;
		method_vec.resize(__argc-2);
		for (int i = 0; i < __argc - 2; i++) {
			method_vec[i] = atoi(__argv[i+2]);
			//method[i] = 2;
		}
		std::cout << "モード:csv統合"  << std::endl;
	}
	int MatchUp_Count=0;

	clock_t Start_Main = clock();
	//実験用対戦相手学習
	if (mode == 0) {
		/*
		Make_Directory_AIT(0,trial);
		FloreMethods Flore_1;
		for (int t = 4; t < trial; t++) {
			Flore_1.Main_Tasks();
			Flore_1.Fwrite_Floreano(t);
		}
		*/
	}
	//学習で記録したデータをもとに実際に対戦
	else if (mode == 1) {
		/*
		Match m("AI", Main_Method, 0, Main_KO, KOT, Main_KU, Main_PER, Main_K);
		Make_CSV_Directory(Main_Method);
		if (!m.output_ni_csv(Main_Trial)) {
			std::cout << "error : output_ni_csv" << std::endl;
			exit(0);
		}
		for (int Opp_t = 0; Opp_t < 5; Opp_t++) {
			//現手法vsFloreano 世代数:2000(100世代間隔) 現手法集団50 Floreano集団30
			m.main_task(Main_Trial, Opp_t);
		}
		*/
	}

	//現手法学習
	// DIR, KO, KU, PER, K, PARENT, CHILD
	else if (mode == 2) {
		//現手法
		if (method == 0) 
		{
			/*
			Coans_GT2016 Coans_GT2016("AI", Main_KO, Main_KU, Main_PER, Main_K, Main_PARENT, Main_CHILD);
			Coans_GT2016.Coans_GT2016_Tasks(Main_Trial);
			MatchUp_Count = Coans_GT2016.Get_MatchUp_Num();
			*/
		}
		//階層的クラスタリングを盛り込んだ手法
		if (method == 1) 
		{
			coans_mode1 Mode1("nim", ku, per, trial);
			Mode1.main_task();
			MatchUp_Count = Mode1.Get_MatchUp_Num();
		}
	}
	//csv統合
	else if (mode == 3) {
		/*
		CsvModules_Intend Method;
		Method.Create_Data(method, KO, Main_Trial, KU, PER);
		*/
	}
	//テスト
	else if (mode == 4) {
		/*
		if (Main_Method == 1) {
			CoansMode1 Mode1("AI", Main_KO, Main_KU, Main_PER, Main_PARENT, Main_CHILD);
			Mode1.Stra_nitch_CSV(Main_Trial);
		}
		if (Main_Method == 2) {
			CoansMode2 Mode2("AI", Main_KO, Main_KU, Main_PER, Main_PARENT, Main_CHILD);
			Mode2.Stra_nitch_CSV(Main_Trial);
		}
		*/
	}

	clock_t End_Main = clock();
	Show_Time(Start_Main, End_Main);
	std::cout << "match_up num:" << MatchUp_Count << std::endl;

	return 0;				// ソフトの終了
}

void Show_Time(clock_t Start, clock_t End) {
	int time = (Start - Start) / CLOCKS_PER_SEC;
	std::cout << "Process time:" << time << "[sec]" << std::endl;
}