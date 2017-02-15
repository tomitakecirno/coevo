#pragma once
/*-------------------------
下村君のゲームへ適用するときに
共通して必要になる関数や構造体
-------------------------*/
#include<vector>
#include <algorithm>
#include <numeric>
#include "getrandcoans.hpp"
#include "Source.hpp"

//富田手法で使う構造体
struct playerTK {
	int x = 0;
	int y = 0;
	int attackstate = 0;
	int movestate = 0;
	int hitstate = 0;
	int direction = 0;
	int movecount = 0;
	int attackcount = 0;
	int hitcount = 0;
	int imagecount = 0;
	int delaycount = 0;
	int hitlimit = 0;
	int hp = 300;
	int combo = 1;
	int fallspeed = 0;
	double eval;
	int nitch;
	std::vector<double> Result; //対戦結果
	std::vector<int> List1;
	std::vector<int> List2;
	std::vector< std::vector<double> > w1_CO;
	std::vector< std::vector<double> > w2_CO;
	std::vector< std::vector<double> > w3_CO;
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
	x = 0;
	y = 0;
	attackstate = 0;
	movestate = 0;
	hitstate = 0;
	direction = 0;
	movecount = 0;
	attackcount = 0;
	hitcount = 0;
	imagecount = 0;
	delaycount = 0;
	hitlimit = 0;
	hp = 300;
	combo = 1;
	fallspeed = 0;
	eval = 0;
	nitch = 0;
	Result.clear();
	List1.clear();
	List2.clear();
}

/**************
解以外を初期化
**************/
void playerTK::Init_0()
{
	using namespace std;
	w1_CO.resize(I1);
	for (int i = 0; i < I1; i++) {
		w1_CO[i].resize(J1);
		for (int j = 0; j < J1; j++) {
			w1_CO[i][j] = 0;
		}
	}
	w2_CO.resize(I2);
	for (int i = 0; i < I2; i++) {
		w2_CO[i].resize(J2);
		for (int j = 0; j < J2; j++) {
			w2_CO[i][j] = 0;
		}
	}
	w3_CO.resize(J1);
	for (int i = 0; i < J1; i++) {
		w3_CO[i].resize(I2);
		for (int j = 0; j < I2; j++) {
			w3_CO[i][j] = 0;
		}
	}
}

/**************
解を初期化
**************/
void playerTK::Init_w()
{
	using namespace std;
	vector<vector<double> > tmpw1(I1);
	vector<vector<double> > tmpw2(I2);
	vector<vector<double> > tmpw3(J1);
	w1_CO.resize(I1);
	for (int i = 0; i < I1; i++) {
		w1_CO[i].resize(J1);
		for (int j = 0; j < J1; j++) {
			w1_CO[i][j] = GetRand_Real(10);
		}
	}
	w2_CO.resize(I2);
	for (int i = 0; i < I2; i++) {
		w2_CO[i].resize(J2);
		for (int j = 0; j < J2; j++) {
			w2_CO[i][j] = GetRand_Real(10);
		}
	}
	w3_CO.resize(J1);
	for (int i = 0; i < J1; i++) {
		w3_CO[i].resize(I2);
		for (int j = 0; j < I2; j++) {
			w3_CO[i][j] = GetRand_Real(10);
		}
	}
}

/*********************************
子個体の評価値．
**********************************/
void FitnessChild(playerTK &child, std::vector<playerTK> &oppoment, bool s2)
{
	double child_win = 0;
	double opponent_win = 0;

	int childResultLength = child.Result.size();
	if (s2) {
		for (int i = 0; i<childResultLength; i++) {
			opponent_win = 0;
			int oppomentResultLength = oppoment[i].Result.size();
			for (int j = 0; j<oppomentResultLength; j++) {
				opponent_win += oppoment[i].Result[j];
			}
			child_win += child.Result[i] * fabs(opponent_win);
		}
	}
	else
	{
		for (int i = 0; i<childResultLength; i++) {
			child_win += child.Result[i];
		}
	}
	child.eval = child_win;
}

/*********************************
自分の戦略の情報を移す関数．
**********************************/
void StrategySet_M(const playerTK &pop) {
	for (int i = 0; i < I1; i++) {
		for (int j = 0; j < J1; j++) {
			w1[i][j] = pop.w1_CO[i][j];
		}

	}
	for (int i = 0; i < I2; i++) {
		for (int j = 0; j < J2; j++) {
			w2[i][j] = pop.w2_CO[i][j];
			
		}

	}
	for (int i = 0; i < I2; i++) {
		for (int j = 0; j < J1; j++) {
			w3[i][j] = pop.w3_CO[i][j];
		}

	}
}

/*********************************
自分の戦略の情報を移す関数．
**********************************/
void StrategySet_T(playerTK &oppoment) {
	for (int i = 0; i < I1; i++) {
		for (int j = 0; j < J1; j++) {
			w1_T[i][j] = oppoment.w1_CO[i][j];
		}

	}
	for (int i = 0; i < I2; i++) {
		for (int j = 0; j < J2; j++) {
			w2_T[i][j] = oppoment.w2_CO[i][j];

		}

	}
	for (int i = 0; i < I2; i++) {
		for (int j = 0; j < J1; j++) {
			w3_T[i][j] = oppoment.w3_CO[i][j];
		}
	}
}