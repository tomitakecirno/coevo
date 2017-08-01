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
#include "CrusteringMethod.h"
#include "Usual_Methods.hpp"
#include <time.h>
#include <string>

/*��@�̃e���v���[�g*/
class Coans{
//���J�����o
public :
	//��@�̃��C�����[�`��
	void	Coans_Tasks(int Trial);
	//�ΐ�񐔂��擾
	int		Get_MatchUp_Num();
	//�헪����������
	void	File_Write_Pop(int trial, int gene, bool s1);
	//�ȉ������p
	void	SetVec_Cr_Pop(std::vector<playerTK> &Pop);	//�N���X�^�ԍ���CSV�N���X��
	void	SetVec_Re_Pop(std::vector<playerTK> &Pop);	//�W�c�̑ΐ팋�ʂ��L�^
	void	SetVec_Re_Opp(std::vector<playerTK> &Opp);	//����W�c�̑ΐ팋�ʂ�CSV�N���X��
//����J�����o
private:
	virtual void Crustering() = 0;
	virtual void Generate_Opp() = 0;
	virtual void Return_Re(int Child_Num, int Opp_Num) = 0;
	virtual void Cal_Fitness() = 0;
	//���̃N���X�Ɣh���N���X����A�N�Z�X�\
protected:
	int Machup_Num;						//�ΐ��
	int Cr_Num;							//�N���X�^�[�ԍ�
	int Opp_Num;						//�ΐ��
	int MainParent;						//��e
	int Method_Num;						//��@��r�̎��Ɏg��
	int	Cru_K;							//�N���X�^�����O�p�����[�^�[
	std::string Dir;
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

	//�W�c�錾
	Pop.resize(KO);
	init_genrand((unsigned)time(NULL)); /*����������*/

	//�W�c������
	for (int i = 0; i < KO; i++) {
		Pop[i].Init();
		Pop[i].Init_w();
	}
	Make_Directory(Dir, Method_Num, Trial, KU, PER, Cru_K);
	CsvModules Csv1(Method_Num);
	Csv1.Init(Method_Num, Trial, KU, PER, Cru_K);
	Cr_Pop.resize(KO);
	Re_Pop.resize(KO);

	//��������̐헪���L�^
	File_Write_Pop(Trial, 0, true);
	Machup_Num = 0;
	//�C�ӂ̐��㐔���[�v
	int Loop_Time_Start;
	int Loop_Time_End;
	int Loop_Time;
	Crustering();
	exit(0);
	for (int Gene_Loop = 1; Gene_Loop < KU+1; Gene_Loop++) {
		Loop_Time_Start = clock();
		std::cout << Method_Num << ":" << Trial << ":" << Cru_K << ":" << Gene_Loop;
		std::cout << "  |  ";
		std::cout << "1" << ',';
		Crustering(); //�N���X�^�����O�B��@�ɂ���ĕς��
		//�����p
		if (Gene_Loop % PER == 0 || Gene_Loop == 1) {
			std::cout << "9" << ',';
			//�W�c�̃N���X�^�ԍ����x�N�^�[�Ɋi�[
			SetVec_Cr_Pop(Pop);
			//�i�[�����x�N�^�[��Csv�N���X��
			Csv1.SetCsv_Cr_P(Cr_Pop);
		}
		else {
			SetVec_Cr_Pop(Pop);
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
				Opponent[i].Result.resize(CHILD+1);
			}
			for (int i = 0; i < CHILD+1; i++) {
				Child[i].Result.resize(opponentLength);
			}
			//�����őΐ�
			for (int r = 0; r < opponentLength; r++) {//���胋�[�v
				StrategySet_T(Opponent[r]);
				for (int c = 0; c < CHILD + 1; c++) {//�W�c���[�v
					StrategySet_M(Child[c]);
					Machup_Num++;
					Competition();//�ΐ� player1 = �q�� palyer2 = �ΐ푊��H
					Return_Re(c, r);
				}
			}
			std::cout << "6" << ',';
			//�K���x�v�Z
			Cal_Fitness();

			std::cout << "7" << ',';
			//Best�̂��W�c��
			int Index = Choice_Best_Index(Child, false);
			Pop[MainParent] = Child[Index];

			//�����p
			if (Gene_Loop % PER == 0) {
				std::cout << "10" << ',';
				//100���㖈�ɐ헪���L�^
				File_Write_Pop(Trial, Gene_Loop / PER, true);
			}
			std::cout << "8";
			std::cout << "  [";
			for (int c = 0; c < CHILD + 1; c++) {
				std::cout << Child[c].eval << ",";
			}
			std::cout << "]" << std::endl;
			//�W�c�̉��ȊO������
			for (int i = 0; i < KO; i++) {
				Pop[i].Init();
			}
			/*
			std::cout << "  [";
			int Cr_Pop_Length = int(Cr_Pop.size());
			for (int cp = 0; cp < Cr_Pop_Length; cp++) {
				std::cout << Cr_Pop[cp] << ",";
			}
			std::cout << "]" << std::endl;
			*/
		}
		Loop_Time_End = clock();
		Loop_Time = (Loop_Time_End - Loop_Time_Start) / CLOCKS_PER_SEC;
		std::cout << "Time per gene : " << Loop_Time << " [sec]" << std::endl;
		assert( Gene_Loop <= KU );
	}
	Csv1.Fwrite_Cr_P();
	//Csv1.Fwrite_Re_P(C);
}
void Coans::SetVec_Cr_Pop(std::vector<playerTK> &Pop){
	//������
	Cr_Pop.assign(KO, 0);
	for (int i = 0; i < KO; i++) {
		assert( Pop[i].nitch < KO );
		assert( 0 <= Pop[i].nitch );
		Cr_Pop[ Pop[i].nitch ]++;
	}
	Cr_Pop.erase( std::remove(Cr_Pop.begin(), Cr_Pop.end(), 0), Cr_Pop.end() );
	assert( !Cr_Pop.empty() );
	/*
	std::cout << "[";
	int Cr_Pop_Length = int(Cr_Pop.size());
	for (int cp = 0; cp < Cr_Pop_Length; cp++) {
		std::cout << Cr_Pop[cp] << ",";
	}
	std::cout << "]" << std::endl;
	*/
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
void Coans::File_Write_Pop(int trial, int gene, bool s1)
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
	char filename[50];
	for (int i = 0; i < KO; i++) {
		if (Method_Num == 0) {
			sprintf(filename, ("AI/%d/%d/%d/%d.dat"), Method_Num, trial, gene, i);
		}
		else if (0 < Method_Num) {
			sprintf(filename, ("AI/%d/%d/%d/%d/%d.dat"), Method_Num, trial, Cru_K, gene, i);
		}
		if ((fp = fopen(filename, "wb+")) == NULL) {
			fprintf(stderr, "%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		/*
		else {
			sprintf(filename, ("AIT/%d/%d.dat"), trial, i);
			if ((fp = fopen(filename, "wb+")) == NULL) {
				fprintf(stderr, "%s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
		}
		*/
		fwrite(w1_GA[i], sizeof(double), I1*J1, fp);
		fwrite(w2_GA[i], sizeof(double), I2*J2, fp);
		fwrite(w3_GA[i], sizeof(double), I2*J1, fp);
		fclose(fp);
	}
}

/*
	mode0
	�ΐ푊����e�N���X�^����̌̂Ɍ���
	�]���͏��������̐�
	�]���l�ɕ␳��������
	List1,2���p
*/
class CoansMode0 : public Coans {
public:
	CoansMode0(std::string str, int k = 0){
		Method_Num = 0;
		Dir = str;
		Cru_K = k;
	}
private :
	virtual void Crustering();
	virtual void Generate_Opp();
	virtual void Return_Re(int ChildNumber, int Opp_Num);
	virtual void Cal_Fitness();
};
void CoansMode0::Crustering() {
	//�ߖT���X�g�������N���X�^�ԍ�����U��
	MakeList(Pop,true,true,false);
	Cr_Num = 1;
	for (int i = 0; i < KO; i++) {
		if (SetNitch(Cr_Num, i, Pop) == 1) {
			Cr_Num++;
		}
	}
}
void CoansMode0::Generate_Opp() {
	choice_oppoment(Pop, Opponent, Cr_Num);
	Opp_Num = Cr_Num;
}
void CoansMode0::Return_Re(int Child_Num, int Opp_Num) {
	Opponent[Opp_Num].Result[Child_Num] = player2.hp - player1.hp;
	Child[Child_Num].Result[Opp_Num] = player1.hp - player2.hp;
}
void CoansMode0::Cal_Fitness() {
	int Length = int(Child.size());
	for (int i = 0; i < Length; i++) {
		FitnessChild(Child[i], Opponent, false);
	}
}


/*
	mode1
	�E�H�[�h�@�̊K�w�I�N���X�^�����O�Ŗ����I�ɃN���X�^�����O
	�]����HP�̍���
*/
class CoansMode1 : public Coans {
public:
	CoansMode1(std::string str, int k = 0) {
		Method_Num = 1;
		Dir = str;
		Cru_K = k;
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
	�ΐ푊����e�N���X�^����̌̂Ɍ���
	�]���͏��������̐�
	�]���l�ɕ␳��������
	�N���X�^�����O�ɊK�w�I�Ȏ�@�𓱓�
	���X�g��3�ɂ��Ă݂�
*/
class CoansMode2 : public Coans {
public:
	CoansMode2(std::string str, int k = 0) {
		Method_Num = 2;
		Dir = str;
		Cru_K = k;
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
	MakeList(Pop, false, false, true);
	//�N���X�^�ԍ�����U��
	Cru_Upgma(Pop, Cru_K);
	Cr_Num = Cru_K;
}
void CoansMode2::Generate_Opp() {
	choice_oppoment(Pop, Opponent, Cru_K);
	Opp_Num = Cru_K;
}
void CoansMode2::Return_Re(int Child_Num, int Opp_Num) {
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
void CoansMode2::Cal_Fitness() {
	int Length = int(Child.size());
	for (int i = 0; i < Length; i++) {
		FitnessChild(Child[i], Opponent, false);
	}
}

/*
mode3
����:List1
�ΐ푊��͎��W�c�S��
�]����HP�̍���
�ΐ팋�ʂ��g���ĕ]���l�ɕ␳�������Ă���
*/
class CoansMode3 : public Coans {
public:
	CoansMode3(std::string str, int k = 0) {
		Method_Num = 3;
		Dir = str;
		Cru_K = k;
	}
private:
	virtual void Crustering();
	virtual void Generate_Opp();
	virtual void Return_Re(int ChildNumber, int Opp_Num);
	virtual void Cal_Fitness();
};
void CoansMode3::Crustering() {
	//�ߖT���X�g�������N���X�^�ԍ�����U��
	MakeList(Pop, true, true, false);
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
	Opponent[Opp_Num].Result[Child_Num] = player2.hp - player1.hp;
	Child[Child_Num].Result[Opp_Num] = player1.hp - player2.hp;
}
void CoansMode3::Cal_Fitness() {
	int Length = int(Child.size());
	for (int i = 0; i < Length; i++) {
		FitnessChild(Child[i], Opponent, false);
	}
}

/*
mode4
����:List1
�ΐ푊��͎��W�c�S��
HP�̍���K���x�Ƃ���
�K���x�ɕ␳��������
*/

class CoansMode4 : public Coans {
public:
	CoansMode4(std::string str, int k = 0) {
		Method_Num = 4;
		Dir = str;
		Cru_K = k;
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
	MakeList(Pop, true, true, false);
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
	Opponent[Opp_Num].Result[Child_Num] = (player2.hp - player1.hp) / 300;
	Child[Child_Num].Result[Opp_Num] = (player1.hp - player2.hp) / 300;
}
void CoansMode4::Cal_Fitness() {
	int Length = int(Child.size());
	for (int i = 0; i < Length; i++) {
		FitnessChild(Child[i], Opponent, true);
	}
}