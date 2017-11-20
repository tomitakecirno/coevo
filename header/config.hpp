#pragma once
/* #define NDEBUG */
/*-------------------------
下村君のゲームへ適用するときに
共通して必要になる関数や構造体
-------------------------*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include "Usual_Methods.hpp"
#include "Source.hpp"
#include <fstream>
#include <cassert>

#define K_List1	10	/*ニッチの集団数*/
#define K_List2	2	/*ニッチの集団数*/
#define TRIAL	10	/*ニッチの集団数*/
#define KL1		2	/*ニッチの集団数*/

#define F_TRIAL	5	/*ニッチの集団数*/
#define FLORET	5	/*ニッチの集団数*/
#define F_KU	300	/*ニッチの集団数*/
#define ENEMY	5

#define CSV_DIR "./csv"

double w1_inout[I1][J1];
double w2_inout[I2][J2];
double w3_inout[I2][J1];

struct p_data {
	int win;
	double eval;
	int nitch;
	std::vector<double> Result; //対戦結果
	std::vector<int> List1;
	std::vector<int> List2;
	std::vector<int> List3;
};

//富田手法で使う構造体(格闘ゲーム)
struct playerTK : public p_data
{
	playerTK() {
		flag = 0;
		comp_flag = 0;
		gene_count = 0;
		delete_flag = 0;
	}
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
	int win;
	double eval;
	int nitch;
	std::vector<double> Result; //対戦結果
	std::vector<int> List1;
	std::vector<int> List2;
	std::vector<int> List3;
	std::vector< std::vector<double> > w1_CO;
	std::vector< std::vector<double> > w2_CO;
	std::vector< std::vector<double> > w3_CO;
	int flag;
	int comp_flag;
	int gene_count;
	int delete_flag;
public:
	void Init();
	void Init_0();
	void Init_w();
	void output_stra(std::string fname);
};
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
	List3.clear();
}
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
void playerTK::output_stra(std::string fname) {
	double tmp_w1[I1][J1];
	double tmp_w2[I2][J2];
	double tmp_w3[I2][J1];

	for (int i = 0; i < I1; i++) {
		for (int j = 0; j < J1; j++) {
			tmp_w1[i][j] = w1_CO[i][j];
		}
	}
	for (int i = 0; i < I2; i++) {
		for (int j = 0; j < J2; j++) {
			tmp_w2[i][j] = w2_CO[i][j];
		}
	}
	for (int i = 0; i < I2; i++) {
		for (int j = 0; j < J1; j++) {
			tmp_w3[i][j] = w3_CO[i][j];
		}
	}

	//ファイル書き込み
	FILE *fp;
	if ((fp = fopen(fname.c_str(), "wb+")) == NULL) {
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	fwrite(tmp_w1, sizeof(double), I1*J1, fp);
	fwrite(tmp_w2, sizeof(double), I2*J2, fp);
	fwrite(tmp_w3, sizeof(double), I2*J1, fp);
	fclose(fp);
}

struct playerTK_ex : public playerTK {
	std::vector<int> Result_int;
};

/* 子個体の評価値 */
void FitnessChild(playerTK &child, std::vector<playerTK> &oppoment, bool s2)
{
	double opponent_win = 0;
	child.eval = 0;

	int childResultLength = int(child.Result.size());
	if (s2) {
		for (int i = 0; i<childResultLength; i++) {
			opponent_win = 0;
			int oppomentResultLength = int(oppoment[i].Result.size());
			for (int j = 0; j<oppomentResultLength; j++) {
				opponent_win += oppoment[i].Result[j];
			}
			child.eval += child.Result[i] * opponent_win;
		}
	}
	else
	{
		for (int i = 0; i<childResultLength; i++) {
			child.eval += child.Result[i];
		}
	}
}

/* 自分の戦略の情報を移す関数 */
void StrategySet_M(const playerTK &pop) {
	if (pop.w1_CO.empty() && pop.w2_CO.empty() && pop.w3_CO.empty()) {
		std::cout << "解が存在しません" << std::endl;
		exit(0);
	}
	else {
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
}

/* 自分の戦略の情報を移す関数 */
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

/* 自分の戦略の情報を移す関数 */
void Strategy_to_w(const playerTK &pop) {
	for (int i = 0; i < I1; i++) {
		for (int j = 0; j < J1; j++) {
			w1_inout[i][j] = pop.w1_CO[i][j];
		}

	}
	for (int i = 0; i < I2; i++) {
		for (int j = 0; j < J2; j++) {
			w2_inout[i][j] = pop.w2_CO[i][j];

		}

	}
	for (int i = 0; i < I2; i++) {
		for (int j = 0; j < J1; j++) {
			w3_inout[i][j] = pop.w3_CO[i][j];
		}
	}
}

/* 自分の戦略の情報を移す関数 */
void w_to_Strategy(playerTK &pop) {
	for (int i = 0; i < I1; i++) {
		for (int j = 0; j < J1; j++) {
			pop.w1_CO[i][j] = w1_inout[i][j];
		}

	}
	for (int i = 0; i < I2; i++) {
		for (int j = 0; j < J2; j++) {
			pop.w2_CO[i][j] = w2_inout[i][j];
		}

	}
	for (int i = 0; i < I2; i++) {
		for (int j = 0; j < J1; j++) {
			pop.w3_CO[i][j] = w3_inout[i][j];
		}
	}
}


//最良個体のインデックスを返す
int Choice_Best_Index(std::vector<playerTK> &Child, bool random = true) {
	int Child_Length = int(Child.size());
	std::vector<double> Tmp_Eval(Child_Length);

	//一旦格納
	for (int i = 0; i < Child_Length; i++) {
		Tmp_Eval[i] = Child[i].eval;
	}

	int index = 0;
	auto max = max_element(Tmp_Eval.begin(), Tmp_Eval.end());
	if (Tmp_Eval[0] < *max) {
		//同じ評価地の個体が複数ある場合はランダム
		int count_Num = int(count(Tmp_Eval.begin(), Tmp_Eval.end(), *max));
		//cout << "count_Num:" << count_Num << endl;
		if (random) {
			if (count_Num == 1) {
				//インデックスを取得
				index = int(distance(Tmp_Eval.begin(), max));
			}
			else if (1 < count_Num) {
				std::vector<int> Tmp_Max_Index(count_Num);
				Tmp_Max_Index[0] = int(distance(Tmp_Eval.begin(), max));
				for (int j = 1; j < count_Num; j++) {
					auto Index_Iterator = find(Tmp_Eval.begin() + Tmp_Max_Index[j - 1] + 1, Tmp_Eval.end(), *max);
					Tmp_Max_Index[j] = int(distance(Tmp_Eval.begin(), Index_Iterator));
				}
				index = int(Tmp_Max_Index[GetRand_Int(count_Num)]);
			}
		}
		else {
			index = int(distance(Tmp_Eval.begin(), max));
		}
	}
	else {
		index = 0;
	}
	return index;
}
