#include "../header/coansmodule.hpp"
#include "../header/CoansMethods.h"
#include "../header/Floreano.h"
#include "../header/config.hpp"

using namespace std;

/****************************
メインルーチン
DE
0:対戦相手学習
1:プレイヤーVSプレイヤー
2:重み学習	
***************************/
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

	}
	//メイン集団学習
	else if (DE == 2) {

	}
	CoansMode1 Mode1;
	switch(DE) {
	case 0:
		break;
	case 1:
		gamemode();
		break;
	//学習
	case 2:
		for (int t = 0; t < TRIAL; t++) {
			Mode1.Coans_Tasks(t);
			cout << "対戦回数:" << Mode1.Get_MatchUp_Num() << endl;
		}
		break;
	}
	//プレイ
	WaitKey();		// キー入力待ち
	DxLib_End();			// ＤＸライブラリ使用の終了処理
	return 0;				// ソフトの終了 
}