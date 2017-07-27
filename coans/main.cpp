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

int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (__argc < 4) {
		cout << "not enough Parameter" << endl;
		exit(0);
	}
	int Main_Mode = atoi(__argv[1]);
	int Main_Method = atoi(__argv[2]);
	int Main_Trial = atoi(__argv[3]);
	int Main_K = atoi(__argv[4]);
	/*	
		argv[1]:Method
		argv[2]:Trial
		argv[3]:k
	*/
	cout << "モード:" << Main_Mode << endl;
	cout << "試行回数:" << Main_Trial << endl;
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
		for (int Opp_t = 0; Opp_t < F_TRIAL; Opp_t++) {
			//現手法vsFloreano 世代数:2000(100世代間隔) 現手法集団50 Floreano集団30
			Match_0.Init_Parameter(Main_Method, 0, 50, 30, KU, PER, Main_K);
			Match_0.Match_And_SetDATA(Main_Trial, Opp_t);
			Match_0.File_Write_CSV(Main_Trial, Opp_t);
		}
	}
	//現手法学習
	else if (Main_Mode == 2) {
		//現手法
		if (Main_Method == 0) {
			CoansMode0 Mode0;
			Mode0.Set_C(0);
			Make_Directory_AI(0, Main_Trial, KU, PER);
			Make_CSV_Directory(0);

			clock_t Start_Main = clock();
			Mode0.Coans_Tasks(Main_Trial);
			clock_t End_Main = clock();
			//Mode1.File_Write_Pop(t, true);
			int MatchUp_Count = Mode0.Get_MatchUp_Num();

			int time = (End_Main - Start_Main) / CLOCKS_PER_SEC;
			std::cout << "Process time:" << time << "[sec]" << std::endl;
			std::cout << "match_up num:" << MatchUp_Count << std::endl;
		}
		//階層的クラスタリングを盛り込んだ手法
		else if (Main_Method == 1) {
			CoansMode1 Mode1;
			Mode1.Set_C(Main_K);
			Make_Directory_AI_1(1, Main_Trial, Main_K, KU, PER);
			Make_CSV_Directory(1);

			clock_t Start_Main = clock();
			Mode1.Coans_Tasks(Main_Trial);
			clock_t End_Main = clock();
			//Mode1.File_Write_Pop(t, true);
			int MatchUp_Count = Mode1.Get_MatchUp_Num();

			int time = (End_Main - Start_Main) / CLOCKS_PER_SEC;
			std::cout << "Process time:" << time << "[sec]" << std::endl;
			std::cout << "match_up num:" << MatchUp_Count << std::endl;
		}
		//階層的クラスタリング＋List3
		else if (Main_Method == 2) {
			CoansMode2 Mode2;
			Mode2.Set_C(Main_K);
			Make_Directory_AI_1(2, Main_Trial, Main_K, KU, PER);
			Make_CSV_Directory(2);

			clock_t Start_Main = clock();
			Mode2.Coans_Tasks(Main_Trial);
			clock_t End_Main = clock();
			//Mode1.File_Write_Pop(t, true);
			int MatchUp_Count = Mode2.Get_MatchUp_Num();

			int time = (End_Main - Start_Main) / CLOCKS_PER_SEC;
			std::cout << "Process time:" << time << "[sec]" << std::endl;
			std::cout << "match_up num:" << MatchUp_Count << std::endl;
		}
	}
	//プレイ
	//WaitKey();		// キー入力待ち
	DxLib_End();			// ＤＸライブラリ使用の終了処理
	return 0;				// ソフトの終了 
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