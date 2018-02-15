#pragma once
#include <fstream>
#include <time.h>
#include <cstdlib>
#include <cstring>
//#include "nim.h"
#include "coans_nim.h"
#include "coansmodule_nim.hpp"
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
template<class T>
void show(T value) {
	std::cout << value << ":" << value*2 << std::endl;
}
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
	//method = 6;
	mode = atoi(__argv[1]);
	if (mode == 1 || mode == 2 || mode == 5) {
		switch (__argc) {
		case 1:
			//Debug mode
			std::cout << "Debug mode" << std::endl;
			trial = 0;
			break;
		case 4:
			method = atoi(__argv[2]);
			trial = atoi(__argv[3]);
			break;
/*
		default:
			cout << "パラメーターが足りません" << endl;
			cout << "coans.exe [mode][method][generation][trial]" << endl;
			exit(1);
			break;
			*/
		}
		if (mode == 1) {
			std::cout << "モード:対戦" << std::endl;
		}
		else if (mode == 2) {
			std::cout << "モード:学習" << std::endl;
		}
	}
	if (mode == 3 || mode == 4) {
		//method.resize(1);
		//method[0] = 2;
		method_vec.resize(__argc - 2);
		for (int i = 0; i < __argc - 2; i++) {
			method_vec[i] = atoi(__argv[i + 2]);
			//method[i] = 2;
		}
		//method_vec.push_back(3);
		std::cout << "モード:csv統合" << std::endl;
	}
	__int64 MatchUp_Count = 0;

	const clock_t Start_Main = clock();
	//学習で記録したデータをもとに実際に対戦

	//現手法学習
	// DIR, KO, KU, PER, K, PARENT, CHILD
	if (mode == 2) {
		//現手法
		//階層的クラスタリングを盛り込んだ手法
		if (method == 1)
		{
			std::vector<coans_mode1> mode1(TRIAL, { trial });
			mode1[trial].main_task2();
		}
		if (method == 2)
		{
			std::vector<coans_mode2> mode2(TRIAL, { trial });
			mode2[trial].main_task();
		}
		if (method == 3)
		{
			std::vector<coans_mode3> mode3(TRIAL, { trial });
			mode3[trial].main_task();
		}
		if (method == 4)
		{
			std::vector<mode4> mode4(TRIAL, { trial });
			mode4[trial].main_task2();
		}
		if (method == 5)
		{
			std::vector<mode5> mode5(TRIAL, { trial });
			mode5[trial].main_task();
		}
		if (method == 6)
		{
			std::vector<mode6> mode6(TRIAL, { trial });
			mode6[trial].main_task3();
		}
	}
	//csv統合
	else if (mode == 3) {
		std::cout << "method :";
		show_vec_1(method_vec);
		//exp_opp_stra_disper(method_vec);
		exp_pop_stra_disper(method_vec);
		//exp_BestRate(method_vec);
		//exp_opp_BestRate(method_vec);
		//exp_pop_pseudoF(method_vec);
		//exp_pop_disper(method_vec);
	}
	else if (mode == 4) {
		if (method_vec.empty()) {
			exit(EXIT_FAILURE);
		}
		save_opp_stra(method_vec);
	}
	const clock_t End_Main = clock();
	Show_Time(Start_Main, End_Main);
	std::cout << "match_up num:" << MatchUp_Count << std::endl;

	return 0;				// ソフトの終了
}

void Show_Time(clock_t Start, clock_t End) {
	double time = (End - Start) / double(CLOCKS_PER_SEC);
	std::cout << "Process time:" << time << "[sec]" << std::endl;
}