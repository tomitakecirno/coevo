#pragma once
#include <fstream>
#include <time.h>
#include <cstdlib>
#include <cstring>
#include "nim.h"
#include "coans_nim.h"
#include "coansmodule_nim.hpp"
#include "Floreano_nim.h"
#include "matchmethods_nim.h"


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
	//mode = 2;
	mode = atoi(__argv[1]);
	if (mode == 1 || mode == 2 || mode == 4) {
		switch (__argc) {
		case 1:
			//Debug mode
			method = 2;
			ku = KU;
			trial = 0;
			break;
		case 5:
			method = atoi(__argv[2]);
			ku = atoi(__argv[3]);
			trial = atoi(__argv[4]);
			break;
/*
		default:
			cout << "パラメーターが足りません" << endl;
			cout << "coans.exe [mode][method][generation][trial]" << endl;
			exit(1);
			break;
			*/
		}
		per = ku / 10;
		if (mode == 1) {
			std::cout << "モード:対戦" << std::endl;
		}
		else if (mode == 2) {
			std::cout << "モード:学習" << std::endl;
		}
	}
	if (mode == 3) {
		//method.resize(1);
		//method[0] = 2;
		method_vec.resize(__argc - 2);
		for (int i = 0; i < __argc - 2; i++) {
			method_vec[i] = atoi(__argv[i + 2]);
			//method[i] = 2;
		}
		std::cout << "モード:csv統合" << std::endl;
	}
	int MatchUp_Count = 0;

	const clock_t Start_Main = clock();
	//実験用対戦相手学習
	if (mode == 0) {
		for (int t = 0; t < F_TRIAL; t++) {
			floreano flore(t);
			flore.main_task();
		}
	}
	//学習で記録したデータをもとに実際に対戦
	else if (mode == 1) {
		Match Match(method, 0, trial, ku, per);
		for (int t = 0; t < 5; t++) {
			Match.main_task(t);
		}
	}

	//現手法学習
	// DIR, KO, KU, PER, K, PARENT, CHILD
	else if (mode == 2) {
		//現手法
		//階層的クラスタリングを盛り込んだ手法
		if (method == 2)
		{
			coans_mode2 Mode2("nim", ku, per, trial);
			Mode2.main_task();
			MatchUp_Count = Mode2.Get_MatchUp_Num();
		}
		if (method == 3)
		{
			coans_mode3 Mode3("nim", ku, per, trial);
			Mode3.main_task();
			MatchUp_Count = Mode3.Get_MatchUp_Num();
		}
	}
	//csv統合
	else if (mode == 3) {
		csvmodules_exp Method;
		Method.integration(method_vec, KU, PER);
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
	else if (mode == 5) {
		nim nim;
		nim.nim_game();
	}

	const clock_t End_Main = clock();
	Show_Time(Start_Main, End_Main);
	std::cout << "match_up num:" << MatchUp_Count << std::endl;

	return 0;				// ソフトの終了
}

void Show_Time(clock_t Start, clock_t End) {
	int time = (End - Start) / CLOCKS_PER_SEC;
	std::cout << "Process time:" << time << "[sec]" << std::endl;
}