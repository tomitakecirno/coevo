#pragma once
/************************
numbers���ʂ̐ݒ�
************************/

#include <vector>
#include "getrandcoans.hpp"

#define DEM 2

struct playerTK {
	double n[DEM];	//��
	double eval;	//�]���l
	int nitch;		//�N���X�^�ԍ�
	std::vector<double> Result; //�ΐ팋��
	std::vector<int> List1;
	std::vector<int> List2;
public:
	void Init();
	void Init_0();
	void Init_w();
};


/**************
���ȊO��������
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
����������
**************/
void playerTK::Init_w()
{
	for (int i = 0; i < DEM; i++){
		n[i] = GetRand_Real(100);
	}
}

/*********************************
�q�̂̕]���l�D
**********************************/
void FitnessChildCorrection(playerTK &child, std::vector<playerTK> &oppoment)
{
	int child_win = 0;
	int opponent_win = 0;

	//����W�c�̌̂̂��ꂼ��̏��������߂�
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