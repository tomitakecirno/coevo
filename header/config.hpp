#pragma once
/*-------------------------
�����N�̃Q�[���֓K�p����Ƃ���
���ʂ��ĕK�v�ɂȂ�֐���\����
-------------------------*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include "getrandcoans.hpp"
#include "Source.hpp"
#include <fstream>

//�x�c��@�Ŏg���\����
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
	std::vector<double> Result; //�ΐ팋��
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

/* ���ȊO�������� */
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

/* ���������� */
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

/* ���������� */
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

/* �q�̂̕]���l */
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

/* �����̐헪�̏����ڂ��֐� */
void StrategySet_M(const playerTK &pop) {
	if (pop.w1_CO.empty() && pop.w2_CO.empty() && pop.w3_CO.empty()) {
		std::cout << "�������݂��܂���" << std::endl;
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

/* �����̐헪�̏����ڂ��֐� */
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

//�ŗǌ̂̃C���f�b�N�X��Ԃ�
int Choice_Best_Index(std::vector<playerTK> &Child) {
	int Child_Length = int(Child.size());
	std::vector<double> Tmp_Eval(Child_Length);

	//��U�i�[
	for (int i = 0; i < Child_Length; i++) {
		Tmp_Eval[i] = Child[i].eval;
	}

	int index = 0;
	auto max = max_element(Tmp_Eval.begin(), Tmp_Eval.end());
	//�����]���n�̌̂���������ꍇ�̓����_��
	int count_Num = int(count(Tmp_Eval.begin(), Tmp_Eval.end(), *max));
	//cout << "count_Num:" << count_Num << endl;
	if (count_Num == 1) {
		//�C���f�b�N�X���擾
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
