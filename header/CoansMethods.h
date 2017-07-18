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
	void Coans_Tasks(int Trial);
	//�ΐ�񐔂��擾
	int Get_MatchUp_Num();
	//�헪����������
	void File_Write_Pop(int trial, bool s1);
	//�ȉ������p
	void SetVec_Cr_Pop(std::vector<playerTK> &Pop);	//�N���X�^�ԍ���CSV�N���X��
	void SetVec_Re_Pop(std::vector<playerTK> &Pop);	//�W�c�̑ΐ팋�ʂ��L�^
	void SetVec_Re_Opp(std::vector<playerTK> &Opp);	//����W�c�̑ΐ팋�ʂ�CSV�N���X��
//����J�����o
private:
	virtual void Set_Method() = 0;
	virtual void Crustering() = 0;
	virtual void Generate_Opp() = 0;
	virtual void Return_Re(int Child_Num, int Opp_Num) = 0;
	virtual void Cal_Fitness() = 0;
//���̃N���X�Ɣh���N���X����A�N�Z�X�\
protected:
	int Machup_Num;					//�ΐ��
	int Cr_Num;					//�N���X�^�[�ԍ�
	int Opp_Num;					//�ΐ��
	int MainParent;						//��e
	int Method_Num;						//��@��r�̎��Ɏg��
	std::vector<int> SubParent;			//���e
	std::vector<playerTK> Pop;			//�i��������W�c
	std::vector<playerTK> Child;		//�q
	std::vector<playerTK> Opponent;		//�ΐ푊��
	//�ȉ������p
	std::vector<int> Cr_Pop; //�N���X�^�ԍ����i�[
	std::vector<int> Re_Pop; //���C���̑ΐ팋�ʂ��i�[
	std::vector<int> Re_Opp; //�ΐ푊���
};
/*��@�̃��C�����[�`��*/
void Coans::Coans_Tasks(int Trial)
{
	std::cout << "Initiarize" << std::endl;

	Set_Method();

	//�W�c�錾
	Pop.resize(KO);
	init_genrand((unsigned)time(NULL)); /*����������*/

	//�W�c������
	for (int i = 0; i < KO; i++) {
		Pop[i].Init();
		Pop[i].Init_w();
	}

	std::cout << "Csv class generate" << std::endl;
	CsvModules Csv1;
	Csv1.Init(Trial, KU, PER);
	Cr_Pop.resize(KO);
	Re_Pop.resize(KO);

	Machup_Num = 0;
	//�C�ӂ̐��㐔���[�v
	for (int gene = 0; gene < KU; gene++) {
		std::cout << Trial << ":" << gene;
		std::cout << "  |  ";
		std::cout << "1" << ',';
		Crustering(); //�N���X�^�����O�B��@�ɂ���ĕς��
		//�����p
		if (gene % PER == 0) {
			std::cout << "9" << ',';
			//�W�c�̃N���X�^�ԍ����x�N�^�[�Ɋi�[
			SetVec_Cr_Pop(Pop);
			//�i�[�����x�N�^�[��Csv�N���X��
			Csv1.SetCsv_Cr_P(Cr_Pop);
		}
		int tmpIndex;
		int tmpSub;
		SubParent.resize(0);
		std::cout << "2" << ',';
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

			std::cout << "3" << ',';
			//�g��XLM
			Child[0] = Pop[MainParent];
			ExtensionXLM(MainParent, SubParent, Pop, Child);
			//�ΐ푊��̌̂�I��
			std::cout << "4" << ',';
			Generate_Opp();

			//w1,w2,w3:�q�̂̐헪�Cw1_T,w2_T,w3_T:�ΐ푊��̐헪
			//player1:�q�́Cplayer2:�ΐ푊��0
			std::cout << "5" << ',';
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
					Machup_Num++;
					Competition();//�ΐ� player1 = �q�� palyer2 = �ΐ푊��H

					Return_Re(c,r);
				}
			}
			std::cout << "6" << ',';
			//�K���x�v�Z
			Cal_Fitness();
			for (int i = 0; i < CHILD; i++) {
				FitnessChild(Child[i], Opponent, false);
			}

			std::cout << "7" << ',';
			//Best�̂��W�c��
			int Index = Choice_Best_Index(Child);
			Pop[MainParent] = Child[Index];

			//�����p
			/*
			if (gene % PER == 0) {
				std::cout << "10" << ',';
				//Floreano�Ƒΐ�
				//MachUpFloreano(Pop);
				//�̂̕��Ϗ������x�N�^�[��
				SetVec_Re_Pop(Pop);
				Csv1.SetCsv_Re_P(Re_Pop);
			}
			*/
			std::cout << "8" << ',';
			//�W�c�̉��ȊO������
			for (int i = 0; i < KO; i++) {
				Pop[i].Init();
			}
		}
		std::cout << std::endl;
	}
	Csv1.Fwrite_Cr_P();
	Csv1.Fwrite_Re_P();
}
void Coans::SetVec_Cr_Pop(std::vector<playerTK> &Pop){
	//������
	Cr_Pop.assign(Cr_Num,0);
	for (int i = 0; i < KO; i++) {
		Cr_Pop[ Pop[i].nitch ]++;
	}
	for (int i = 0; i < Cr_Num; i++) {
		if (Cr_Pop[i] == 0) {
			Cr_Pop.erase(Cr_Pop.begin() + i ); //�v�f���폜
		}
	}
}
void Coans::SetVec_Re_Pop(std::vector<playerTK> &Pop) {
	for (int i = 0; i < KO; i++) {
		//�ΐ팋�ʂ̃x�N�^�[�̘a���i�[����
		Re_Pop[i] = int(std::accumulate(Pop[i].Result.begin(), Pop[i].Result.end(), 0.0));
	}
}
void Coans::SetVec_Re_Opp(std::vector<playerTK> &Opp) {
	for (int i = 0; i < Opp_Num; i++) {
		//�ΐ팋�ʂ̃x�N�^�[�̘a���i�[����
		Re_Opp[i] = int(std::accumulate(Opp[i].Result.begin(), Opp[i].Result.end(), 0.0));
	}
}
/*�ΐ�񐔂�Ԃ�*/
int Coans::Get_MatchUp_Num() {
	return Machup_Num;
}
void Coans::File_Write_Pop(int trial, bool s1)
{
	//�l�������킹��
	for (int i = 0; i < KO; i++) {
		for (int j = 0; j < I1; j++) {
			for (int k = 0; k < J1; k++) {
				w1_GA[i][j][k] = Pop[i].w1_CO[j][k];
			}
		}
		for (int j = 0; j < I2; j++) {
			for (int k = 0; k < J2; k++) {
				w2_GA[i][j][k] = Pop[i].w2_CO[j][k];
			}
		}
		for (int j = 0; j < J1; j++) {
			for (int k = 0; k < I2; k++) {
				w3_GA[i][j][k] = Pop[i].w3_CO[j][k];
			}
		}
	}
	//bef = true�̎�AI�Cfalse�̎�AIC
	for (int i = 0; i < KO; i++) {
		if (s1) {
			sprintf(filename, ("AI/%d/%d/%d.dat"), Method_Num, trial, i);
			if ((fp = fopen(filename, "wb+")) == NULL) {
				fprintf(stderr, "%s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
		}
		else {
			sprintf(filename, ("AIT/%d/%d.dat"), trial, i);
			if ((fp = fopen(filename, "wb+")) == NULL) {
				fprintf(stderr, "%s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
		}
		fwrite(w1_GA[i], sizeof(double), I1*J1, fp);
		fwrite(w2_GA[i], sizeof(double), I2*J2, fp);
		fwrite(w3_GA[i], sizeof(double), I2*J1, fp);
		fclose(fp);
	}
}
/*
mode1
�ΐ푊����e�N���X�^����̌̂Ɍ���
�]���͏��������̐�
�]���l�ɕ␳��������
*/
class CoansMode1 : public Coans {
private :
	virtual void Set_Method();
	virtual void Crustering();
	virtual void Generate_Opp();
	virtual void Return_Re(int ChildNumber, int Opp_Num);
	virtual void Cal_Fitness();
};
void CoansMode1::Crustering() {
	//�ߖT���X�g�������N���X�^�ԍ�����U��
	MakeList(Pop);
	Cr_Num = 1;
	for (int i = 0; i < KO; i++) {
		if (SetNitch(Cr_Num, i, Pop) == 1) {
			Cr_Num++;
		}
	}
}
void CoansMode1::Set_Method() {
	Method_Num = 0;
}
void CoansMode1::Generate_Opp() {
	choice_oppoment(Pop, Opponent, Cr_Num);
	Opp_Num = Cr_Num;
}
void CoansMode1::Return_Re(int Child_Num, int Opp_Num) {
	if (player1.win == 1) {
		Opponent[Opp_Num].Result[Child_Num] = 0;
		Child[Child_Num].Result[Opp_Num] = 1;
	}
	else if (player2.win == 1) {
		Opponent[Opp_Num].Result[Child_Num] = 1;
		Child[Child_Num].Result[Opp_Num] = 0;
	}
	else {
		Opponent[Opp_Num].Result[Child_Num] = 0;
		Child[Child_Num].Result[Opp_Num] = 0;
	}
}
void CoansMode1::Cal_Fitness() {
	int Length = int(Child.size());
	for (int i = 0; i < Length; i++) {
		FitnessChild(Child[i], Opponent, false);
	}
}
