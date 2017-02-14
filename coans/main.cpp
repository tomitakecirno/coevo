#include "../header/coansmodule.hpp"
#include "../header/Floreano.h"

using namespace std;

int coans(int mode, int trial);
/****************************
メインルーチン
***************************/
int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int battle_count[4][TRIAL] = { 0 };
	//プレイ
	if (DE == 1) {
		gamemode();
	}
	//??
	else if (DE == 0) {
		FloreanoEval();
	}
	//学習
	else if (DE == 2) {
		for (int t = 0; t < TRIAL; t++) {
			for (int m = 0; m < 1; m++) {
				battle_count[m][t] = coans(m, t);
				cout << battle_count[m][t] << " ";
			}
			//mode:0 現手法：現評価方法(引き分けでポイントを与えない)
			//mode:1 現手法：前評価方法(引き分けでポイントを与える)
			//mode:2 前手法：現評価方法
			//mode:3 前手法：前評価方法
		}
		exit(0);
	}
	else if (DE == 3) {
		exit(0);
	}
	else if (DE == 4) {
		for (int t = 0; t < TRIAL; t++) {
			cout << "試行回数:" << t << endl;
			for (int m = 0; m < 4; m++) {
				ALL(m, 4, t);
			}
		}
	}
	else if (DE == 5) {
		for (int t = 0; t < TRIAL; t++) {
			for (int m = 0; m < 4; m++) {
				ALLWatch(m, t);
			}
		}
	}
	WaitKey();		// キー入力待ち
	DxLib_End();			// ＤＸライブラリ使用の終了処理
	return 0;				// ソフトの終了 
}