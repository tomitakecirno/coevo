#pragma once
/*=======================================
�����̊e���@�܂Ƃ�
CoansMode0	���_�̎�@
CoansMode1	����@�F���]�����@(�]���l�ɕ␳��������)
CoansMode2	����@�F�O�]�����@(�]���l�ɕ␳�������Ȃ�)
CoansMode3	�O��@�F���]�����@
CoansMode4	�O��@�F�O�]�����@
=======================================*/
#include "config.hpp"
#include "coansmodule.hpp"
#include "CsvModules.h"
#include "CrusteringMethod.h"
#include "Usual_Methods.hpp"
#include <time.h>
#include <string>
#include <fstream>

/*��@�̃N���X*/
class Coans{
//���J�����o
public :
	void	Coans_Tasks(int Trial); 	//��@�̃��C�����[�`��
	int		Get_MatchUp_Num();			//�ΐ�񐔂��擾
	
	//�f�[�^���~�X�����Ƃ��Ɏg�����
	void	Stra_nitch_CSV(int Trial);	//�헪�f�[�^��ǂݍ����
	//�ȉ������p
//����J�����o
private:
	virtual void Crustering() = 0;
	virtual void Generate_Opp() = 0;
	virtual void Return_Re(int Child_Num, int Opp_Num) = 0;
	virtual void Cal_Fitness() = 0;
	//���̃N���X�Ɣh���N���X����A�N�Z�X�\
protected:
	int Method_Num;						//��@��r�̎��Ɏg��
	int Pop_n;							//���W�c�̌̐�
	int Gene;							//���㐔
	int Per;							//�Ԋu
	int Pare;							//�e�̐�
	int child;							//�q�̐�
	int Cr_Num;							//�N���X�^�[�ԍ�
	int Opp_Num;
	int	Cru_K;							//�N���X�^�����O�p�����[�^�[
	int MainParent;						//��e
	int trial;

	int Machup_Num;						//�ΐ��
	std::string Dir;
	std::vector<playerTK> Pop;			//�i��������W�c
	std::vector<playerTK> Child;
	std::vector<playerTK> Opponent;		//�ΐ푊��

	void Output_stra(const int index);
};
void Coans::Coans_Tasks(int Trial)
{

	std::cout << "��@:" << Method_Num << std::endl;
	std::cout << "���s��:" << Trial << std::endl;
	std::cout << "�N���X�^��:" << Cru_K << std::endl;
	std::cout << "���㐔:" << Gene << std::endl;
	std::cout << "��؂�:" << Per << std::endl;
	std::cout << "�W�c�̐�:" << Pop_n << std::endl;
	std::cout << "�q�̐�:" << Pare << std::endl;
	std::cout << "�e�̐�:" << child << std::endl;
	trial = Trial;
	//�W�c�錾
	Pop.resize(Pop_n);
	init_genrand((unsigned)time(NULL)); /*����������*/

	//�W�c������
	for (int i = 0; i < Pop_n; i++) {
		Pop[i].Init();
		Pop[i].Init_w();
	}
	Make_Directory(Dir, Method_Num, Trial, Gene, Per, Cru_K);

	//��������̐헪���L�^
	Machup_Num = 0;
	//�C�ӂ̐��㐔���[�v
	int Loop_Time_Start;
	int Loop_Time_End;
	int Loop_Time;
	//Crustering();

	std::cout << "Initiarized..." << std::endl;
	Output_stra(0);
	std::cout << "Strategy0..." << std::endl;

	for (int Gene_Loop = 1; Gene_Loop < Gene + 1; Gene_Loop++) {
		Loop_Time_Start = clock();
		std::cout << Method_Num << ":" << Trial << ":" << Cru_K << ":" << Gene_Loop;
		std::cout << "  |  ";
		std::cout << "1" << ',';
		Crustering(); //�N���X�^�����O�B��@�ɂ���ĕς��
		int tmpIndex;
		int tmpSub;
		std::vector<int> SubParent;
		std::cout << "2" << ',';
		//��e�I��
		MainParent = GetRand_Int(Pop_n);
		//���e��I��
		for (int i = 0; i < Pare; i++) {
			if (Pop[MainParent].List1.empty()) {
				break;
			}
			tmpIndex = GetRand_Int(int(Pop[MainParent].List1.size()));
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
				Opponent[i].Result.resize(CHILD + 1);
			}
			for (int i = 0; i < CHILD + 1; i++) {
				Child[i].Result.resize(opponentLength);
			}
			//�����őΐ�
			const double time_start = clock();
			for (int r = 0; r < 1; r++) {//���胋�[�v
				StrategySet_T(Opponent[r]);
				for (int c = 0; c < CHILD + 1; c++) {//�W�c���[�v
					StrategySet_M(Child[c]);
					Machup_Num++;
					Competition();//�ΐ� player1 = �q�� palyer2 = �ΐ푊��H
					Return_Re(c, r);
				}
			}
			const double time_end = clock();
			const double time = time_end - time_start;
			std::cout << std::endl << "�ΐ�� : " << Machup_Num << std::endl;
			std::cout << "�ΐ펞�� : " << time / CLOCKS_PER_SEC << "[sec]" << std::endl;
			std::cout << "6" << ',';
			//�K���x�v�Z
			Cal_Fitness();

			std::cout << "7" << ',';
			//Best�̂��W�c��
			int Index = Choice_Best_Index(Child, false);
			Pop[MainParent] = Child[Index];

			//�����p
			if (Gene_Loop % Per == 0) {
				std::cout << "10" << ',';
				//100���㖈�ɐ헪���L�^
				Output_stra(Gene_Loop / Per);
			}
			std::cout << "8";
			/*
			std::cout << "  [";
			for (int c = 0; c < CHILD + 1; c++) {
				std::cout << Child[c].eval << ",";
			}
			std::cout << "]" << std::endl;
			*/
			//�W�c�̉��ȊO������
			for (int i = 0; i < Pop_n; i++) {
				Pop[i].Init();
			}
		}
		Loop_Time_End = clock();
		Loop_Time = (Loop_Time_End - Loop_Time_Start) / CLOCKS_PER_SEC;
		std::cout << "Time per gene : " << Loop_Time << " [sec]" << std::endl;
	}
}
int Coans::Get_MatchUp_Num() {
	return Machup_Num;
}
void Coans::Output_stra(const int index) {

	char tmp_fname[50];
	sprintf(tmp_fname, "./%s/%d/%d/%d/", Dir.c_str(), Method_Num, trial, index);

	std::stringstream fname;
	for (int i = 0; i < KO; i++) {
		fname << tmp_fname << i << ".dat";
		Pop[i].output_stra(fname.str());
		fname.str("");
		fname.clear(std::stringstream::goodbit);
	}
}
void Coans::Stra_nitch_CSV(int Trial) {
	/*
	std::cout << "���[�h:" << 4 << std::endl;
	std::cout << "��@:" << Method_Num << std::endl;
	std::cout << "���s��:" << Trial << std::endl;
	std::cout << "�N���X�^��:" << Cru_K << std::endl;
	std::cout << "���㐔:" << Gene << std::endl;
	std::cout << "��؂�:" << Per << std::endl;
	std::cout << "�W�c�̐�:" << Pop_n << std::endl;

	//Init
	Pop.resize(Pop_n);
	for (int i = 0; i < Pop_n; i++) {
		Pop[i].Init();
		Pop[i].Init_0();
	}
	Csv_exp csv_exp(Dir, Method_Num, Trial, Gene, Per, Pare, child);
	for (int g = 0; g < Gene / Per + 1; g++) {
		//�헪�i�[
		for (int i = 0; i < Pop_n; i++) {
			FILE *file2;
			char fname[50];
			if (Cru_K == 0) {
				sprintf_s(fname, ("%s/%d/%d/%d/%d.dat"), Dir.c_str(), Method_Num, Trial, g, i);
			}
			else if (0 < Cru_K) {
				sprintf_s(fname, ("%s/%d/%d/%d/%d/%d.dat"), Dir.c_str(), Method_Num, Trial, Cru_K, g, i);
			}
			if ((file2 = fopen(fname, "rb")) == NULL) {
				std::cout << "Main file open error!! :" << fname << std::endl;
				exit(0);
			}
			//���v���C���[�̐헪�i�[
			fread(w1_inout, sizeof(double), I1 * J1, file2);
			fread(w2_inout, sizeof(double), I2 * J2, file2);
			fread(w3_inout, sizeof(double), I2 * J1, file2);
			fclose(file2);
			w_to_Strategy(Pop[i]);
		}
		//�N���X�^�����O
		Crustering();
		csv_exp.SetVec_Cr_Pop(Pop, g);
	}
	csv_exp.fwrite_Cr_P();
	*/
}

/*
	mode1
	�E�H�[�h�@�̊K�w�I�N���X�^�����O�Ŗ����I�ɃN���X�^�����O
	�]����HP�̍���
*/
class CoansMode1 : public Coans {
public:
	CoansMode1(std::string str, int n, int g, int g_p, int p, int c, int k = 0) {
		Method_Num = 1;					//��@��r�̎��Ɏg��
		Pop_n = n;						//���W�c�̌̐�
		Gene = g;						//���㐔
		Per = g_p;						//�Ԋu
		Pare = p;						//�e�̐�
		child = c;						//�q�̐�
		Machup_Num = 0;					//�ΐ��
		Opp_Num = 0;
		Cru_K = k;						//�N���X�^�����O�p�����[�^�[
		Dir = str;
	}
private:
	//�N���X�^�����O�Ɏg���p�����[�^�[
	virtual void Crustering();
	virtual void Generate_Opp();
	virtual void Return_Re(int ChildNumber, int Opp_Num);
	virtual void Cal_Fitness();
};
void CoansMode1::Crustering() {
	//�ߖT���X�g����
	//MakeList(Pop, true, false, false);
	//�N���X�^�ԍ�����U��
	Cru_Upgma(Pop, Cru_K);
	Cr_Num = Cru_K;
}
void CoansMode1::Generate_Opp() {
	choice_oppoment(Pop, Opponent, Cru_K);
	Opp_Num = Cru_K;
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

/*
	mode2
	����	�FList1
	�ΐ푊��F�e�N���X�^�̑�\
	�K���x	�F�����Ƒ����HP�̍���
*/
class CoansMode2 : public Coans {
public:
	CoansMode2(std::string str, int n, int g, int g_p, int p, int c, int k = 0) {
		Method_Num = 2;					//��@��r�̎��Ɏg��
		Pop_n = n;						//���W�c�̌̐�
		Gene = g;						//���㐔
		Per = g_p;						//�Ԋu
		Pare = p;						//�e�̐�
		child = c;						//�q�̐�
		Machup_Num = 0;					//�ΐ��
		Opp_Num = 0;
		Cru_K = k;						//�N���X�^�����O�p�����[�^�[
		Dir = str;
	}
private:
	//�N���X�^�����O�Ɏg���p�����[�^�[
	virtual void Crustering();
	virtual void Generate_Opp();
	virtual void Return_Re(int ChildNumber, int Opp_Num);
	virtual void Cal_Fitness();
};
void CoansMode2::Crustering() {
	//�ߖT���X�g����
	MakeList(Pop, K_List1, K_List2, 0);
	Cr_Num = 1;
	for (int i = 0; i < KO; i++) {
		if (SetNitch(Cr_Num, i, Pop) == 1) {
			Cr_Num++;
		}
	}
}
void CoansMode2::Generate_Opp() {
	choice_oppoment(Pop, Opponent, Cr_Num);
	Opp_Num = Cr_Num;
}
void CoansMode2::Return_Re(int Child_Num, int Opp_Num) {
	Opponent[Opp_Num].Result[Child_Num] = (player2.hp - player1.hp) / 300;
	Child[Child_Num].Result[Opp_Num] = (player1.hp - player2.hp) / 300;
}
void CoansMode2::Cal_Fitness() {
	int Length = int(Child.size());
	for (int i = 0; i < Length; i++) {
		FitnessChild(Child[i], Opponent, false);
	}
}

/*
	mode3
	����	�FList1
	�ΐ푊��F�W�c�S��
	�K���x	�F�����Ƒ����HP�̍���
*/
class CoansMode3 : public Coans {
public:
	CoansMode3(std::string str, int n, int g, int g_p, int p, int c, int k = 0) {
		Method_Num = 3;					//��@��r�̎��Ɏg��
		Pop_n = n;						//���W�c�̌̐�
		Gene = g;						//���㐔
		Per = g_p;						//�Ԋu
		Pare = p;						//�e�̐�
		child = c;						//�q�̐�
		Machup_Num = 0;					//�ΐ��
		Opp_Num = 0;
		Cru_K = k;						//�N���X�^�����O�p�����[�^�[
		Dir = str;
	}
private:
	virtual void Crustering();
	virtual void Generate_Opp();
	virtual void Return_Re(int ChildNumber, int Opp_Num);
	virtual void Cal_Fitness();
};
void CoansMode3::Crustering() {
	//�ߖT���X�g�������N���X�^�ԍ�����U��
	MakeList(Pop, K_List1, K_List2, 0);
	Cr_Num = 1;
	for (int i = 0; i < KO; i++) {
		if (SetNitch(Cr_Num, i, Pop) == 1) {
			Cr_Num++;
		}
	}
}
void CoansMode3::Generate_Opp() {
	Opponent = Pop;
}
void CoansMode3::Return_Re(int Child_Num, int Opp_Num) {
	Opponent[Opp_Num].Result[Child_Num] = (player2.hp - player1.hp) / 300;
	Child[Child_Num].Result[Opp_Num] = (player1.hp - player2.hp) / 300;
}
void CoansMode3::Cal_Fitness() {
	int Length = int(Child.size());
	for (int i = 0; i < Length; i++) {
		FitnessChild(Child[i], Opponent, false);
	}
}

/*
	mode4
	����	�FList1
	�ΐ푊��F�W�c�S��
	�K���x	�F�����Ƒ����HP�̍����{�K���x�ɒǉ��␳
*/
class CoansMode4 : public Coans {
public:
	CoansMode4(std::string str, int n, int g, int g_p, int p, int c, int k = 0) {
		Method_Num = 4;					//��@��r�̎��Ɏg��
		Pop_n = n;						//���W�c�̌̐�
		Gene = g;						//���㐔
		Per = g_p;						//�Ԋu
		Pare = p;						//�e�̐�
		child = c;						//�q�̐�
		Machup_Num = 0;					//�ΐ��
		Opp_Num = 0;
		Cru_K = k;						//�N���X�^�����O�p�����[�^�[
		Dir = str;
	}
private:
	virtual void Set_Method();
	virtual void Crustering();
	virtual void Generate_Opp();
	virtual void Return_Re(int ChildNumber, int Opp_Num);
	virtual void Cal_Fitness();
};
void CoansMode4::Crustering() {
	//�ߖT���X�g�������N���X�^�ԍ�����U��
	MakeList(Pop, K_List1, K_List2, 0);
	Cr_Num = 1;
	for (int i = 0; i < KO; i++) {
		if (SetNitch(Cr_Num, i, Pop) == 1) {
			Cr_Num++;
		}
	}
}
void CoansMode4::Set_Method() {
	Method_Num = 4;
}
void CoansMode4::Generate_Opp() {
	Opponent = Pop;
}
void CoansMode4::Return_Re(int Child_Num, int Opp_Num) {
	Opponent[Opp_Num].Result[Child_Num] = (player2.hp - player1.hp)/300;
	Child[Child_Num].Result[Opp_Num] = (player1.hp - player2.hp)/300;
}
void CoansMode4::Cal_Fitness() {
	int Length = int(Child.size());
	for (int i = 0; i < Length; i++) {
		FitnessChild(Child[i], Opponent, true);
	}
}


/*
	���_��@�̃N���X
*/
/*
class Coans_GT2016 {
//���J�����o
public:
	Coans_GT2016(std::string str, int n, int g, int g_p, int p, int c, int k = 0) {
		Method_Num = 0;					//��@��r�̎��Ɏg��
		Pop_n = n;							//���W�c�̌̐�
		Gene = g;							//���㐔
		Per = g_p;							//�Ԋu
		Pare = p;							//�e�̐�
		Child = c;							//�q�̐�
		Machup_Num = 0;					//�ΐ��
		Opp_Num = 0;					
		Cru_K = k;						//�N���X�^�����O�p�����[�^�[
		Dir = str;		
	}
	//��@�̃��C�����[�`��
	void	Coans_GT2016_Tasks(int Trial);
	int		Get_MatchUp_Num();
	//�헪����������
//����J�����o
protected:
	int Method_Num;						//��@��r�̎��Ɏg��
	int Pop_n;							//���W�c�̌̐�
	int Gene;							//���㐔
	int Per;							//�Ԋu
	int Pare;							//�e�̐�
	int Child;							//�q�̐�
	int Machup_Num;						//�ΐ��
	int Cr_Num;							//�N���X�^�[�ԍ�
	int Opp_Num;						
	int	Cru_K;							//�N���X�^�����O�p�����[�^�[
	int MainParent;						//��e
	std::string Dir;
	std::vector<playerTK> Pop;			//�i��������W�c
	std::vector<playerTK> Opponent;		//�ΐ푊��
	void Update_Opponent(playerTK child);
	int	 Cal_Gra_Nitch(playerTK child);
	//�ȉ������p
	std::vector<int> Cr_Pop;			//�N���X�^�ԍ����i�[
	std::vector<int> Re_Pop;			//���C���̑ΐ팋�ʂ��i�[
	std::vector<int> Re_Opp;			//�ΐ푊���
};
void  Coans_GT2016::Coans_GT2016_Tasks(int Trial)
{
	std::cout << "Initiarize" << std::endl;

	//�W�c�錾
	Pop.resize(Pop_n);
	Opponent.resize(Pop_n);
	init_genrand((unsigned)time(NULL));

										//�W�c������
	for (int i = 0; i < Pop_n; i++) {
		Pop[i].Init();
		Pop[i].Init_w();
	}
	for (int i = 0; i < Pop_n; i++) {
		Opponent[i].Init();
		Opponent[i].Init_w();
	}
	Make_Directory(Dir, Method_Num, Trial, Gene, Per, Cru_K);
	Csv_exp csv_exp(Dir, Method_Num, Trial, Gene, Per, Pare, Child);

	//��������̐헪���L�^
	//�C�ӂ̐��㐔���[�v
	int Loop_Time_Start;
	int Loop_Time_End;
	int Loop_Time;

	MakeList(Opponent, 0, K_List2, 0);
	Cr_Num = 1;
	for (int i = 0; i < Pop_n; i++) {
		if (SetNitch(Cr_Num, i, Opponent) == 1) {
			Cr_Num++;
		}
	}

	for (int Gene_Loop = 1; Gene_Loop < Gene + 1; Gene_Loop++) {
		Loop_Time_Start = clock();
		std::cout << Method_Num << ":" << Trial << ":" << Cru_K << ":" << Gene_Loop;
		std::cout << "  |  ";
		std::cout << "1" << ',';

		//�N���X�^�����O
		MakeList(Pop, K_List1, 0, 0);
		std::cout << "1-1" << ',';

		int tmpIndex;
		int tmpSub;
		std::vector<int> SubParent;	//���e
		std::cout << "2" << ',';
		//��e�I��
		MainParent = GetRand_Int(Pop_n);
		//���e��I��
		for (int i = 0; i < Pare; i++) {
			if (Pop[MainParent].List1.empty()) {
				break;
			}
			tmpIndex = GetRand_Int(int(Pop[MainParent].List1.size()));
			tmpSub = Pop[MainParent].List1[tmpIndex];
			SubParent.push_back(tmpSub); //�v�f��ǉ�
			Pop[MainParent].List1.erase(Pop[MainParent].List1.begin() + tmpIndex); //�ǉ������v�f���폜
		}
		//���e������Έȉ��̏������s��
		if (!SubParent.empty()) {
			//�q�̐���
			std::vector<playerTK> Child(Child + 1);
			Child[0] = Pop[MainParent];
			int child_size = int(Child.size());

			std::cout << "3" << ',';
			//�g��XLM
			ExtensionXLM(MainParent, SubParent, Pop, Child);
			//�ΐ푊��̌̂�I��
			std::cout << "4" << ',';

			//w1,w2,w3:�q�̂̐헪�Cw1_T,w2_T,w3_T:�ΐ푊��̐헪
			//player1:�q�́Cplayer2:�ΐ푊��0
			std::cout << "5" << ',';
			//����W�c�Ƃ̑ΐ팋�ʂ��i�[����x�N�^�[�����T�C�Y
			for (int i = 0; i < Pop_n; i++) {
				Opponent[i].Result.resize(child_size);
			}
			for (int i = 0; i < child_size; i++) {
				Child[i].Result.resize(Pop_n);
			}
			//�����őΐ�
			for (int r = 0; r < Pop_n; r++) {//���胋�[�v
				StrategySet_T(Opponent[r]);
				for (int c = 0; c < child_size; c++) {//�W�c���[�v
					StrategySet_M(Child[c]);
					Machup_Num++;
					Competition();//�ΐ� player1 = �q�� palyer2 = �ΐ푊��H
					Opponent[r].Result[c] = (player2.hp - player1.hp) / 300;
					Child[c].Result[r] = (player1.hp - player2.hp) / 300;
				}
			}
			std::cout << "6" << ',';
			//�K���x�v�Z
			for (int i = 0; i < child_size; i++) {
				FitnessChild(Child[i], Opponent, false);
			}
			std::cout << "7" << ',';
			//Best�̂��W�c��
			int Index = Choice_Best_Index(Child, false);
			//Index = 3;
			//Best �� Child[Index];

			std::cout << "8" << ',';
			//�폜�\��̌̂������
			if (Index != 0) {
				int k;
				for (k = 0; k < Pop_n; k++) {
					if (Opponent[k].delete_flag != 0) {
						break;
					}
				}
				if (k != Pop_n) {
					Opponent[k] = Child[Index];
					Opponent[k].comp_flag = 1;
					Opponent[k].delete_flag = 0;
				}if (k == Pop_n) {
					std::cout << "8-1" << ',';
					Update_Opponent(Child[Index]);
				}
			}
			std::cout << "9" << ',';
			for (int i = 0; i < Pop_n; i++) {
				if (Opponent[i].delete_flag != 1) {
					if (Opponent[i].comp_flag == 0) {
						Opponent[i].gene_count++;
					}
					else if (Opponent[i].comp_flag == 1) {
						Opponent[i].gene_count = 0;
					}
					Opponent[i].comp_flag = 0;
					if (DELETE <= Opponent[i].gene_count) {
						Opponent[i].delete_flag = 1;
						Opponent[i].nitch = 0;
					}
				}
			}
			//�����p
			if (Gene_Loop % Per == 0) {
				std::cout << "11" << ',';
				//100���㖈�ɐ헪���L�^
				csv_exp.Stra_Output_Pop(Pop, Gene_Loop / Per);
			}
			std::cout << "10" << ',';
			std::cout << "  [";
			for (int c = 0; c < child_size; c++) {
				std::cout << Child[c].eval << ",";
			}
			std::cout << "]" << std::endl;
			//�W�c�̉��ȊO������
			for (int i = 0; i < Pop_n; i++) {
				Pop[i].Init();
			}
			std::cout << "  [";
			int Cr_Pop_Length = int(Cr_Pop.size());
			for (int cp = 0; cp < Cr_Pop_Length; cp++) {
				std::cout << Cr_Pop[cp] << ",";
			}
			std::cout << "]" << std::endl;
		}
		Loop_Time_End = clock();
		Loop_Time = (Loop_Time_End - Loop_Time_Start) / CLOCKS_PER_SEC;
		std::cout << "Time per gene : " << Loop_Time << " [sec]" << std::endl;
	}
}
int	  Coans_GT2016::Get_MatchUp_Num() {
	return Machup_Num;
}
void  Coans_GT2016::Update_Opponent(playerTK child)
{
	int Nit_Count = 0;
	int Nit_Num = Cal_Gra_Nitch(child);
	//��ԋ߂��N���X�^�̔ԍ�������
	child.nitch = Nit_Num;

	for (int i = 0; i<KO; i++) {
		if (Opponent[i].nitch == Nit_Num) {
			Opponent[i].eval = 0;
			Opponent[i].flag = 0;
			Nit_Count++;
		}
	}
	//�ΐ�p�̃x�N�^�[�����
	std::vector<playerTK> Oppc(Nit_Count + 1);
	std::vector<double> Oppc_Eval(Nit_Count + 1);
	for (int i = 0; i < KO; i++) {
		if (Opponent[i].nitch == Nit_Num) {
			Oppc[Nit_Count] = Opponent[i];
			Nit_Count--;
		}
	}
	assert(0 <= Nit_Count);
	child.eval = 0;
	child.flag = 0;
	Oppc[0] = child;

	int Oppc_size = int(Oppc.size());
	for (int i = 0; i < Oppc_size; i++) {
		//���N���X�^���̌̂Ƒΐ�
		Oppc[i].Result.resize(Oppc_size - 1);
		StrategySet_M(Oppc[i]);
		for (int j = i + 1; j < Oppc_size - 1; j++) {
			StrategySet_T(Oppc[j]);
			Competition();//�ΐ� player1 = �q�� palyer2 = �ΐ푊��H
			Oppc[i].Result[j] = (player1.hp - player2.hp) / 300;
			Oppc[j].Result[i] = (player2.hp - player1.hp) / 300;
			Machup_Num++;
		}
		//�q�̂Ƒΐ�
		Opponent[i].flag = 1; 
		Opponent[i].comp_flag = 1;
	}
	for (int i = 0; i < Oppc_size; i++) {
		Oppc_Eval[i] = std::accumulate(Oppc[i].Result.begin(), Oppc[i].Result.end(), 0.0);
	}

	auto min = min_element(Oppc_Eval.begin(), Oppc_Eval.end());
	int min_index = int(distance(Oppc_Eval.begin(), min));

	int j;
	for (int i = 0; i < Oppc_size; i++) {
		if (min_index != i) {
			for (j = 0; j < KO && Opponent[j].nitch != Nit_Num; j++) {}
			Opponent[j] = Oppc[i];
			Opponent[j].comp_flag = 1;
			Opponent[j].delete_flag = 0;
		}
	}
	
	for (int i = 0; i < KO; i++) {
		Opponent[i].flag = 0;
		Opponent[i].eval = 0;
		Opponent[i].nitch = 0;
		Opponent[i].List2.clear();
	}
	MakeList(Opponent, 0, K_List2, 0);
	Cr_Num = 1;
	for (int i = 0; i < KO; i++) {
		if (SetNitch(Cr_Num, i, Opponent) == 1) {
			Cr_Num++;
		}
	}
}
int   Coans_GT2016::Cal_Gra_Nitch(playerTK child)
{
	std::vector<int> Nitch_Num;
	std::vector< std::vector < std::vector<double> > > Sum_N_W1;
	std::vector< std::vector < std::vector<double> > > Sum_N_W2;
	std::vector< std::vector < std::vector<double> > > Sum_N_W3;

	Nitch_Num = std::vector<int>(Cr_Num, 0);
	Sum_N_W1 = std::vector<std::vector<std::vector<double>>>(Cr_Num + 1, std::vector<std::vector<double>>(I1, std::vector<double>(J1, 0.0)));
	Sum_N_W2 = std::vector<std::vector<std::vector<double>>>(Cr_Num + 1, std::vector<std::vector<double>>(I2, std::vector<double>(J2, 0.0)));
	Sum_N_W3 = std::vector<std::vector<std::vector<double>>>(Cr_Num + 1, std::vector<std::vector<double>>(J1, std::vector<double>(I2, 0.0)));

	for (int Opp = 0; Opp < KO; Opp++) {
		//�e�̉��̂��ꂼ��̃x�N�g���𑫂�
		//w1_CO
		int Opp_Nitch = Opponent[Opp].nitch;
		//std::cout << "Opp:" << Opp << ',';
		for (int j = 0; j < I1; j++) {
			for (int k = 0; k < J1; k++) {
				Sum_N_W1[Opp_Nitch][j][k] += Opponent[Opp].w1_CO[j][k];
			}
		}
		//w2_CO
		for (int j = 0; j < I2; j++) {
			for (int k = 0; k < J2; k++) {
				Sum_N_W2[Opp_Nitch][j][k] += Opponent[Opp].w2_CO[j][k];
			}
		}
		//w3_CO
		for (int j = 0; j < J1; j++) {
			for (int k = 0; k < I2; k++) {
				Sum_N_W3[Opp_Nitch][j][k] += Opponent[Opp].w3_CO[j][k];
			}
		}
		Nitch_Num[Opp_Nitch]++;
	}
	//std::cout << "8-1-2" << ',';
	//�x�N�g���̏d�S�����߂�
	//w1_CO
	for (int Nit = 1; Nit < Cr_Num; Nit++) {
		for (int j = 0; j < I1; j++) {
			for (int k = 0; k < J1; k++) {
				Sum_N_W1[Nit][j][k] = Sum_N_W1[Nit][j][k] / Nitch_Num[Nit];
			}
		}
		//w2_CO
		for (int j = 0; j < I2; j++) {
			for (int k = 0; k < J2; k++) {
				Sum_N_W2[Nit][j][k] = Sum_N_W2[Nit][j][k] / Nitch_Num[Nit];
			}
		}
		//w3_CO
		for (int j = 0; j < J1; j++) {
			for (int k = 0; k < I2; k++) {
				Sum_N_W3[Nit][j][k] = Sum_N_W3[Nit][j][k] / Nitch_Num[Nit];
			}
		}
	}
	//std::cout << "8-1-3" << ',';
	//�d�S�Ǝq�̂̋����v�Z
	double cal_sum = 0;
	std::vector<double> Dis_Vector(Cr_Num);
	//w1�̋���
	for (int Nit = 1; Nit < Cr_Num; Nit++) {
		for (int j = 0; j < I1; j++) {
			for (int k = 0; k < J1; k++) {
				Dis_Vector[Nit] += (Sum_N_W1[Nit][j][k] - child.w1_CO[j][k])*(Sum_N_W1[Nit][j][k] - child.w1_CO[j][k]);
			}
		}
		//w2�̋���
		for (int j = 0; j < I2; j++) {
			for (int k = 0; k < J2; k++) {
				Dis_Vector[Nit] += (Sum_N_W2[Nit][j][k] - child.w2_CO[j][k])*(Sum_N_W2[Nit][j][k] - child.w2_CO[j][k]);
			}
		}
		//w3�̋���
		for (int j = 0; j < J1; j++) {
			for (int k = 0; k < I2; k++) {
				Dis_Vector[Nit] += (Sum_N_W3[Nit][j][k] - child.w3_CO[j][k])*(Sum_N_W3[Nit][j][k] - child.w3_CO[j][k]);
			}
		}
		Dis_Vector[Nit] = sqrt(Dis_Vector[Nit]);
	}
	//std::cout << "8-1-4" << ',';
	auto min = min_element(Dis_Vector.begin()+1, Dis_Vector.end());
	auto min_Nit = int(distance(Dis_Vector.begin(), min));
	return min_Nit;
}
*/