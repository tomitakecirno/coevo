#pragma once
/************************
numbers共通の設定
************************/

#include <vector>
#include "getrandcoans.hpp"

#define DEM 2

struct playerTK {
	double n[DEM];	//解
	double eval;	//評価値
	int nitch;		//クラスタ番号
	std::vector<double> Result; //対戦結果
	std::vector<int> List1;
	std::vector<int> List2;
public:
	void Init();
	void Init_0();
	void Init_w();
};


/**************
解以外を初期化
**************/
void playerTK::Init()
{
	eval = 0;
	nitch = 0;
	Result.clear();
	List1.clear();
	List2.clear();
}

/**************
解を初期化
**************/
void playerTK::Init_w()
{
	for (int i = 0; i < DEM; i++){
		n[i] = GetRand_Real(100);
	}
}

/*********************************
子個体の評価値．
**********************************/
void FitnessChildCorrection(playerTK &child, std::vector<playerTK> &oppoment)
{
	int child_win = 0;
	int opponent_win = 0;

	//相手集団の個体のそれぞれの勝数を求める
	int childResultLength = child.Result.size();
	for (int i = 0; i<childResultLength; i++) {
		opponent_win = 0;
		int oppomentResultLength = oppoment[i].Result.size();
		for (int j = 0; j<oppomentResultLength; j++) {
			opponent_win += oppoment[i].Result[j];
		}
		child_win += child.Result[i] * opponent_win;
	}
	child.eval = child_win;
}