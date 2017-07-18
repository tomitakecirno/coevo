#include "../header/coansmodule.hpp"
#include "../header/CoansMethods.h"
#include "../header/Floreano.h"
#include "../header/config.hpp"
#include <fstream>
#include <time.h>

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
	//実験用対戦相手学習
	if (DE == 0) {
		FloreMethods Flore_1;
		for (int i = 0; i < TRIAL; i++) {
			Flore_1.Main_Tasks();
			Flore_1.Fwrite_Floreano();
			if (i == TRIAL) {
				break;
			}
		}
	}
	//学習で記録したデータをもとに実際に対戦
	else if (DE == 1) {
		gamemode();
	}
	//メイン集団学習
	else if (DE == 2) {
		std::vector<int> MatchUp_Count(TRIAL);
		CoansMode1 Mode1;
		for (int t = 0; t < TRIAL; t++) {
			std::cout << "trial:" << t << "  ";
			clock_t start = clock();
			Mode1.Coans_Tasks(t);
			clock_t end = clock();
			Mode1.File_Write_Pop(t, true);
			MatchUp_Count[t] = Mode1.Get_MatchUp_Num();

			int time = (end - start) / CLOCKS_PER_SEC;
			int hour = time / 3600;
			int minute = time / 3600 / 60;
			int second = time % 60;
			std::cout << "Process time:" << oct << showbase << hour << ':';
			std::cout << oct << showbase << minute << ':';
			std::cout << oct << showbase << second << std::endl;
			std::cout << "match_up num:" << MatchUp_Count[t] << std::endl;
		}
		//対戦数書き込み
		MatchUp_CSV(MatchUp_Count);
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
