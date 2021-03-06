#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include "../header/Usual_Methods.hpp"

#define DEM		100	//次元数
#define KO		10	//初期集団数
#define KL1		4	//クラスタ関係のパラメーター
#define KL2		3	//クラスタ関係のパラメーター
#define	K_CR	3	//階層型クラスターのパラメーター

class Player {
public:
	void Init(int d);
	void Init_WS();
	int Cr_Num;
	double eval;
	std::vector<double> Stra;
	std::vector<double> Result;
};

class PlayerCoans : public Player {
public:
	void Init_List();
	std::vector<double> List1;
	std::vector<double> List2;
};

//解を初期化
void Player::Init(int d) {
	Stra.resize(d);
	for (int i = 0; i < d; i++) {
		Stra[i] = GetRand_Real(100);
	}
}
//解以外を初期化
void Player::Init_WS() {
	Cr_Num = 0;
	eval = 0;
	Stra.clear();
	Result.clear();
}

void PlayerCoans::Init_List() {
	List1.clear();
	List2.clear();
}

/* 子個体の評価値 */
void FitnessChild(Player &child, std::vector<Player> &oppoment, bool s2)
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
			child.eval += child.Result[i] * fabs(opponent_win);
		}
	}
	else
	{
		for (int i = 0; i<childResultLength; i++) {
			child.eval += child.Result[i];
		}
	}
}

//最良個体のインデックスを返す
int Choice_Best_Index(std::vector<Player> &Child) {
	int Child_Length = int(Child.size());
	std::vector<double> Tmp_Eval(Child_Length);

	//一旦格納
	for (int i = 0; i < Child_Length; i++) {
		Tmp_Eval[i] = Child[i].eval;
	}

	int index = 0;
	auto max = max_element(Tmp_Eval.begin(), Tmp_Eval.end());
	//同じ評価地の個体が複数ある場合はランダム
	int count_Num = int(count(Tmp_Eval.begin(), Tmp_Eval.end(), *max));
	//cout << "count_Num:" << count_Num << endl;
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
	return index;
}

void Show_Vector(std::vector<int> &vec) {
	int vec_length = int(vec.size());
	std::cout << '[';
	for (int v = 0; v < vec_length; v++) {
		std::cout << vec[v] << ',';
	}
	std::cout << ']';
	std::cout << std::endl;
}

void Show_Distance(std::vector<std::vector<double> > &vec1) {
	int vec1_length = int(vec1.size());

	for (int v = 0; v < vec1_length; v++) {
		int vec2_length = int(vec1[v].size());
		std::cout << "[";
		for (int w = 0; w < vec2_length; w++) {
			std::cout << vec1[v][w] << "--";
		}
		std::cout << "]" << std::endl;
	}
}