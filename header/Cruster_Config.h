#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include "../header/getrandcoans.hpp"

#define DEM 2	//������
#define KO	30	//�����W�c��
#define KL1	4	//�N���X�^�֌W�̃p�����[�^�[
#define KL2	3	//�N���X�^�֌W�̃p�����[�^�[

class Player {
public:
	void Init();
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

//����������
void Player::Init() {
	Stra.resize(DEM);
	for (int i = 0; i < DEM; i++) {
		Stra[i] = GetRand_Real(100);
	}
}
//���ȊO��������
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

/* �q�̂̕]���l */
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

//�ŗǌ̂̃C���f�b�N�X��Ԃ�
int Choice_Best_Index(std::vector<Player> &Child) {
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
