#pragma once
/*=======================================
�����̊e���@�܂Ƃ�
CoansMode1	����@�F���]�����@(�]���l�ɕ␳��������)
CoansMode2	����@�F�O�]�����@(�]���l�ɕ␳�������Ȃ�)
CoansMode3	�O��@�F���]�����@
CoansMode4	�O��@�F�O�]�����@
=======================================*/
#include "config.hpp"
#include "coansmodule.hpp"
#include "CsvModules.h"

/*��@�̃e���v���[�g*/
class Coans{
//���J�����o
public :
	//��@�̃��C�����[�`��
	void CoansTasks(int Trial);
	//�ΐ�񐔂��擾
	int GetMatchUpN();
	//�ȉ������p
	void SetData_Cruster(std::vector<playerTK> &Pop);	//�N���X�^�ԍ���CSV�N���X��
	void SetData_PopResult(std::vector<playerTK> &Pop);	//�W�c�̑ΐ팋�ʂ��L�^
	void SetData_OppResult(std::vector<playerTK> &Opp);	//����W�c�̑ΐ팋�ʂ�CSV�N���X��
//����J�����o
private:
	virtual void Crustering() = 0;
	virtual void GenerateOpponent() = 0;
	virtual void ReturnResult(int ChildNumber, int OpponentNumber) = 0;
	virtual void CalculateFitness() = 0;
//���̃N���X�Ɣh���N���X����A�N�Z�X�\
protected:
	int BattleNumber;					//�ΐ��
	int CrusterNumber;					//�N���X�^�[�ԍ�
	int OpponentNumber;					//�ΐ��
	int MainParent;						//��e
	std::vector<int> SubParent;			//���e
	std::vector<playerTK> Pop;			//�i��������W�c
	std::vector<playerTK> Child;		//�q
	std::vector<playerTK> Opponent;		//�ΐ푊��
	//�ȉ������p
	std::vector<int> PopCruster;
	std::vector<int> PopResult;
	std::vector<int> OppResult;
};
/*��@�̃��C�����[�`��*/
void Coans::CoansTasks(int Trial)
{
	//�W�c�錾
	Pop.resize(KO);
	init_genrand((unsigned)time(NULL)); /*����������*/

	//�W�c������
	for (int i = 0; i < KO; i++) {
		Pop[i].Init();
		Pop[i].Init_w();
	}

	CsvModules Csv1;
	Csv1.Init(Trial, KU, PER);
	PopCruster.resize(KO);
	PopResult.resize(KO);

	BattleNumber = 0;
	//�C�ӂ̐��㐔���[�v
	for (int e = 0; e < KU; e++) {
		std::cout << "���s��:" << Trial << "  ";
		std::cout << "���㐔:" << e << std::endl;

		Crustering(); //�N���X�^�����O�B��@�ɂ���ĕς��
		//�����p
		/*
		if (e / PER == 0) {
			SetData_Cruster(Pop);
			Csv1.Csv_SetPopCruster(PopCruster);
		}
		*/
		int tmpIndex;
		int tmpSub;
		SubParent.resize(0);
		//��e�I��
		MainParent = GetRand_Int(KO);
		//���e��I��
		for (int i = 0; i < KL1; i++) {
			if (Pop[MainParent].List1.empty()) {
				break;
			}
			tmpIndex = GetRand_Int( int(Pop[MainParent].List1.size()) );
			tmpSub = Pop[MainParent].List1[tmpIndex];
			SubParent.push_back(tmpSub); //�v�f��ǉ�
			Pop[MainParent].List1.erase(Pop[MainParent].List1.begin() + tmpIndex); //�ǉ������v�f���폜
		}
		//���e������Έȉ��̏������s��
		if (!SubParent.empty()) {
			//�q�̐���
			Child.resize(CHILD + 1);

			//�g��XLM
			Child[0] = Pop[MainParent];
			ExtensionXLM(MainParent, SubParent, Pop, Child);
			//�ΐ푊��̌̂�I��
			GenerateOpponent();

			//w1,w2,w3:�q�̂̐헪�Cw1_T,w2_T,w3_T:�ΐ푊��̐헪
			//player1:�q�́Cplayer2:�ΐ푊��0
			int opponentLength = int(Opponent.size());

			for (int i = 0; i < opponentLength; i++) {
				Opponent[i].Result.resize(CHILD);
			}
			for (int i = 0; i < CHILD; i++) {
				Child[i].Result.resize(opponentLength);
			}
			//�����őΐ�
			for (int r = 0; r < opponentLength; r++) {//���胋�[�v
				StrategySet_T(Opponent[r]);
				for (int c = 0; c < CHILD; c++) {//�W�c���[�v
					StrategySet_M(Child[c]);
					BattleNumber++;
					Competition();//�ΐ� player1 = �q�� palyer2 = �ΐ푊��H

					ReturnResult(c,r);
				}
			}

			//�K���x�v�Z
			CalculateFitness();
			for (int i = 0; i < CHILD; i++) {
				FitnessChild(Child[i], Opponent, false);
			}

			//Best�̂��W�c��
			int Index = 0;
			double Max = Child[0].eval;

			int Length = int(Child.size());
			for (int i = 1; i < Length; i++) {
				if (Max < Child[i].eval) {
					Index = i;
					Max = Child[i].eval;
				}
			}
			Pop[MainParent] = Child[Index];

			//�����p
			/*
			if (e / PER == 0) {
				//���ȊO��������
				for (int i = 1; i < KO; i++) {
					Pop[i].Init();
				}
				//Floreano�Ƒΐ�
				//MachUpFloreano(Pop);
				//�̂̕��Ϗ������x�N�^�[��
				SetData_PopResult(Pop);
				Csv1.Csv_SetPopResult(PopResult);
			}
			*/
			//�W�c�̉��ȊO������
			for (int i = 0; i < KO; i++) {
				Pop[i].Init();
			}
		}
	}
	//Csv1.FileWrite_PopCruster();
	//Csv1.FileWrite_PopResult();
}
void Coans::SetData_Cruster(std::vector<playerTK> &Pop){
	for (int i = 0; i < KO; i++) {
		PopCruster[i] = Pop[i].nitch;
	}
}
void Coans::SetData_PopResult(std::vector<playerTK> &Pop) {
	for (int i = 0; i < KO; i++) {
		//�ΐ팋�ʂ̃x�N�^�[�̘a���i�[����
		PopResult[i] = int(std::accumulate(Pop[i].Result.begin(), Pop[i].Result.end(), 0.0));
	}
	std::cout << "SetData_PopResult��:" << std::endl;
	for (int i = 0; i < KO; i++) {
		std::cout << PopResult[i] << ',';
	}
	std::cout << std::endl;
}
void Coans::SetData_OppResult(std::vector<playerTK> &Opp) {
	for (int i = 0; i < OpponentNumber; i++) {
		//�ΐ팋�ʂ̃x�N�^�[�̘a���i�[����
		OppResult[i] = int(std::accumulate(Opp[i].Result.begin(), Opp[i].Result.end(), 0.0));
	}
}
/*�ΐ�񐔂�Ԃ�*/
int Coans::GetMatchUpN() {
	return BattleNumber;
}

/*
mode1
�ΐ푊����e�N���X�^����̌̂Ɍ���
�]���͏��������̐�
�]���l�ɕ␳��������
*/
class CoansMode1 : public Coans {
private :
	virtual void Crustering();
	virtual void GenerateOpponent();
	virtual void ReturnResult(int ChildNumber, int OpponentNumber);
	virtual void CalculateFitness();
};
void CoansMode1::Crustering() {
	//�ߖT���X�g�������N���X�^�ԍ�����U��
	MakeList(Pop);
	CrusterNumber = 1;
	for (int i = 0; i < KO; i++) {
		if (SetNitch(CrusterNumber, i, Pop) == 1) {
			CrusterNumber++;
		}
	}
}
void CoansMode1::GenerateOpponent() {
	choice_oppoment(Pop, Opponent, CrusterNumber);
	OpponentNumber = CrusterNumber;
}
void CoansMode1::ReturnResult(int ChildNumber, int OpponentNumber) {
	if (player1.win == 1) {
		Opponent[OpponentNumber].Result[ChildNumber] = 0;
		Child[ChildNumber].Result[OpponentNumber] = 1;
	}
	else if (player2.win == 1) {
		Opponent[OpponentNumber].Result[ChildNumber] = 1;
		Child[ChildNumber].Result[OpponentNumber] = 0;
	}
	else {
		Opponent[OpponentNumber].Result[ChildNumber] = 0;
		Child[ChildNumber].Result[OpponentNumber] = 0;
	}
}
void CoansMode1::CalculateFitness() {
	int Length = int(Child.size());
	for (int i = 0; i < Length; i++) {
		FitnessChild(Child[i], Opponent, false);
	}
}
