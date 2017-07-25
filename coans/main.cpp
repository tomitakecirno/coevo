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
2:重み学習	
***************************/
void MatchUp_CSV(std::vector<int> &Count);

int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (__argc < 2) {
		cout << "Please Set Trial" << endl;
		exit(0);
	}
	int Main_Mode = atoi(__argv[1]);
	int Main_Trial = atoi(__argv[2]);

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
		exit(0);
	}
	//学習で記録したデータをもとに実際に対戦
	else if (Main_Mode == 1) {
		gamemode();
	}
	//メイン集団学習
	else if (Main_Mode == 2) {
		std::vector<int> MatchUp_Count(Main_Trial);
		CoansMode1 Mode1;
		Make_Directory_AI(0, Main_Trial, KU, PER);
		Make_CSV_Directory();

		clock_t Start_Main = clock();
		Mode1.Coans_Tasks(Main_Trial);
		clock_t End_Main = clock();
		//Mode1.File_Write_Pop(t, true);
		MatchUp_Count[Main_Trial] = Mode1.Get_MatchUp_Num();

		int time = (End_Main - Start_Main) / CLOCKS_PER_SEC;
		std::cout << "Process time:" << time << "[sec]" << std::endl;
		std::cout << "match_up num:" << MatchUp_Count[Main_Trial] << std::endl;

		//対戦数書き込み
		MatchUp_CSV(MatchUp_Count);
		exit(0);
	}
	//学習で記録したデータをもとに対戦.
	else if (Main_Mode == 3) {
		Match Match_1;
		for (int Opp_t = 0; Opp_t < F_TRIAL; Opp_t++) {
			//現手法vsFloreano 世代数:2000(100世代間隔) 現手法集団50 Floreano集団30
			Match_1.Init_Parameter(0, 0, 50, 30, KU, PER);
			Match_1.Match_And_SetDATA(Main_Trial, Opp_t);
			Match_1.File_Write_CSV(Main_Trial, Opp_t);
		}
		exit(0);
	}
	//プレイ
	WaitKey();		// キー入力待ち
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
