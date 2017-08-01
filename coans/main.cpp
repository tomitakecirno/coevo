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

int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int Main_Mode;
	int Main_Method;
	int Main_Trial;
	int Main_K;
	if (__argc < 3) {
		cout << "not enough Parameter" << endl;
		exit(1);
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

	/*	
		argv[1]:Method
		argv[2]:Trial
		argv[3]:k
	*/
	cout << "モード:" << Main_Mode << endl;
	cout << "手法:" << Main_Method << endl;
	cout << "試行回数:" << Main_Trial << endl;
	cout << "クラスタ数:" << Main_K << endl;
	cout << "世代数:" << KU << endl;
	cout << "区切り:" << PER << endl;

	int MatchUp_Count;

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
		Match Match_0;
		Make_CSV_Directory(Main_Method);
		Match_0.Init_Parameter(Main_Method, 0, 50, 30, KU, PER, Main_K);

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
			CoansMode0 Mode0("AI", Main_Method);
			Mode0.Coans_Tasks(Main_Trial);
			MatchUp_Count = Mode0.Get_MatchUp_Num();
		}
		//階層的クラスタリングを盛り込んだ手法
		else if (Main_Method == 1) {
			CoansMode1 Mode1("TEST", Main_Method);
			Mode1.Coans_Tasks(Main_Trial);
			MatchUp_Count = Mode1.Get_MatchUp_Num();
		}
		//階層的クラスタリング＋List3
		else if (Main_Method == 2) {
			CoansMode2 Mode2("AI", Main_K);
			Mode2.Coans_Tasks(Main_Trial);
			MatchUp_Count = Mode2.Get_MatchUp_Num();
		}else if (Main_Method == 3) {
			CoansMode3 Mode3("AI", Main_K);
			Mode3.Coans_Tasks(Main_Trial);
			MatchUp_Count = Mode3.Get_MatchUp_Num();
		}
		else if (Main_Method == 4) {
			CoansMode4 Mode4("AI", Main_Method);
			Mode4.Coans_Tasks(Main_Trial);
			MatchUp_Count = Mode4.Get_MatchUp_Num();
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