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
	CoansMode1 Mode1;
	switch(DE) {
	case 0:
		FloreanoEval();
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